#include "tilemap.h"

#include <limits.h>

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include "../lib/parson.h"

CollisionRects collisionRects;

CollisionRects GetCollisionRects() {
    return collisionRects;
}

void MergeCollisionTiles(const TileMap *map, CollisionRects *outRects) {
    int w = map->collisionLayer.width;
    int h = map->collisionLayer.height;

    bool *visited = calloc(w * h, sizeof(bool));

    outRects->count = 0;
    outRects->capacity = 64;
    outRects->rects = malloc(sizeof(Rectangle) * outRects->capacity);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int idx = y * w + x;
            if (visited[idx]) continue;
            if (map->collisionLayer.tiles[idx] == 0) continue;

            int xEnd = x;
            while (xEnd + 1 < w && map->collisionLayer.tiles[y * w + (xEnd + 1)] != 0 && !visited[y * w + (xEnd + 1)]) {
                xEnd++;
            }

            int yEnd = y;
            bool canExtend = true;
            while (canExtend && yEnd + 1 < h) {
                for (int xx = x; xx <= xEnd; xx++) {
                    if (map->collisionLayer.tiles[(yEnd + 1) * w + xx] == 0 || visited[(yEnd + 1) * w + xx]) {
                        canExtend = false;
                        break;
                    }
                }
                if (canExtend) yEnd++;
            }

            for (int yy = y; yy <= yEnd; yy++) {
                for (int xx = x; xx <= xEnd; xx++) {
                    visited[yy * w + xx] = true;
                }
            }

            Rectangle rect = {
                x * TILE_SIZE,
                y * TILE_SIZE,
                (xEnd - x + 1) * TILE_SIZE,
                (yEnd - y + 1) * TILE_SIZE
            };

            if (outRects->count >= outRects->capacity) {
                outRects->capacity *= 2;
                outRects->rects = realloc(outRects->rects, sizeof(Rectangle) * outRects->capacity);
            }

            outRects->rects[outRects->count++] = rect;
        }
    }

    free(visited);
}

void LoadLayer(TileLayer *layer, int *tiles, int width, int height) {
    layer->width = width;
    layer->height = height;
    layer->tiles = tiles;
}

void LoadInfiniteLayer(JSON_Object *layerObj, TileLayer *layer) {
    JSON_Array *chunks = json_object_get_array(layerObj, "chunks");
    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;

    size_t chunkCount = json_array_get_count(chunks);
    for (size_t i = 0; i < chunkCount; i++) {
        JSON_Object *chunk = json_array_get_object(chunks, i);
        int x = (int)json_object_get_number(chunk, "x");
        int y = (int)json_object_get_number(chunk, "y");
        int w = (int)json_object_get_number(chunk, "width");
        int h = (int)json_object_get_number(chunk, "height");
        if (x < minX) minX = x;
        if (y < minY) minY = y;
        if (x + w > maxX) maxX = x + w;
        if (y + h > maxY) maxY = y + h;
    }

    int mapWidth = maxX - minX;
    int mapHeight = maxY - minY;
    int *tiles = calloc(mapWidth * mapHeight, sizeof(int));

    for (size_t i = 0; i < chunkCount; i++) {
        JSON_Object *chunk = json_array_get_object(chunks, i);
        int cx = (int)json_object_get_number(chunk, "x") - minX;
        int cy = (int)json_object_get_number(chunk, "y") - minY;
        int cw = (int)json_object_get_number(chunk, "width");
        int ch = (int)json_object_get_number(chunk, "height");

        JSON_Array *data = json_object_get_array(chunk, "data");
        for (int y = 0; y < ch; y++) {
            for (int x = 0; x < cw; x++) {
                int tile = (int)json_array_get_number(data, y * cw + x);
                tiles[(cy + y) * mapWidth + (cx + x)] = tile;
            }
        }
    }

    LoadLayer(layer, tiles, mapWidth, mapHeight);
}

void LoadTileMap(TileMap *map, const char *mapFile, const char *tilesetFile) {
    map->tileset = LoadTexture(tilesetFile);

    JSON_Value *rootValue = json_parse_file(mapFile);
    if (!rootValue) {
        fprintf(stderr, "Failed to parse map file: %s\n", mapFile);
        return;
    }

    JSON_Object *rootObject = json_value_get_object(rootValue);
    JSON_Array *layers = json_object_get_array(rootObject, "layers");

    if (json_array_get_count(layers) < 2) {
        fprintf(stderr, "Map must have at least two layers\n");
        json_value_free(rootValue);
        return;
    }

    LoadInfiniteLayer(json_array_get_object(layers, 0), &map->groundLayer);
    LoadInfiniteLayer(json_array_get_object(layers, 1), &map->collisionLayer);

    json_value_free(rootValue);
    MergeCollisionTiles(map, &collisionRects);
}

void DrawTileMap(const TileMap *map) {
    for (int y = 0; y < map->groundLayer.height; y++) {
        for (int x = 0; x < map->groundLayer.width; x++) {
            int tileIndex = map->groundLayer.tiles[y * map->groundLayer.width + x];
            if (tileIndex > 0) {
                int tsWidth = map->tileset.width / TILE_SIZE;
                Rectangle src = {
                    (float)((tileIndex - 1) % tsWidth) * TILE_SIZE,
                    (float)((tileIndex - 1) / tsWidth) * TILE_SIZE,
                    TILE_SIZE, TILE_SIZE
                };
                Vector2 pos = { x * TILE_SIZE, y * TILE_SIZE };
                DrawTextureRec(map->tileset, src, pos, WHITE);
            }
        }
    }
}

void DrawDebugCollisionRects() {
    for (int i = 0; i < collisionRects.count; i++) {
        DrawRectangleLinesEx(collisionRects.rects[i], 1, RED);
    }
}

bool CheckCollisionWithMergedRects(Rectangle collider) {
    for (int i = 0; i < collisionRects.count; i++) {
        if (CheckCollisionRecs(collider, collisionRects.rects[i])) {
            return true;
        }
    }
    return false;
}
