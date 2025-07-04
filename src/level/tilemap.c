#include "tilemap.h"
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

            // Find horizontal extent
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

            // Mark visited for all these tiles
            for (int yy = y; yy <= yEnd; yy++) {
                for (int xx = x; xx <= xEnd; xx++) {
                    visited[yy * w + xx] = true;
                }
            }

            // Create a rectangle collider
            Rectangle rect = {
                x * TILE_SIZE,
                y * TILE_SIZE,
                (xEnd - x + 1) * TILE_SIZE,
                (yEnd - y + 1) * TILE_SIZE
            };

            // Add to outRects, realloc if needed
            if (outRects->count >= outRects->capacity) {
                outRects->capacity *= 2;
                outRects->rects = realloc(outRects->rects, sizeof(Rectangle) * outRects->capacity);
            }

            outRects->rects[outRects->count++] = rect;
        }
    }

    free(visited);
}


void LoadLayer(TileLayer *layer, const int *data, int width, int height) {
    layer->width = width;
    layer->height = height;

    layer->tiles = malloc(sizeof(int) * width * height);
    if (!layer->tiles) {
        fprintf(stderr, "Failed to allocate memory for tile layer.\n");
        return;
    }

    // Copy tile data
    for (int i = 0; i < width * height; i++) {
        layer->tiles[i] = data[i];
    }
}

void LoadTileMap(TileMap *map, const char *mapFile, const char *tilesetFile) {
    map->tileset = LoadTexture(tilesetFile);

    // Load and parse the JSON map file
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

    // Assuming layer 0 = ground, layer 1 = collision
    JSON_Object *groundLayer = json_array_get_object(layers, 0);
    JSON_Object *collisionLayer = json_array_get_object(layers, 1);

    JSON_Array *groundData = json_object_get_array(groundLayer, "data");
    JSON_Array *collisionData = json_object_get_array(collisionLayer, "data");

    int width = (int)json_object_get_number(groundLayer, "width");
    int height = (int)json_object_get_number(groundLayer, "height");

    int *groundTiles = malloc(sizeof(int) * width * height);
    int *collisionTiles = malloc(sizeof(int) * width * height);

    for (int i = 0; i < width * height; i++) {
        groundTiles[i] = (int)json_array_get_number(groundData, i);
        collisionTiles[i] = (int)json_array_get_number(collisionData, i);
    }

    LoadLayer(&map->groundLayer, groundTiles, width, height);
    LoadLayer(&map->collisionLayer, collisionTiles, width, height);

    free(groundTiles);
    free(collisionTiles);
    json_value_free(rootValue);

    // Merge tiles to big colliders
    MergeCollisionTiles(map, &collisionRects);
}


void DrawTileMap(const TileMap *map) {
    for (int y = 0; y < map->groundLayer.height; y++) {
        for (int x = 0; x < map->groundLayer.width; x++) {
            const int tileIndex = map->groundLayer.tiles[y * map->groundLayer.width + x];
            if (tileIndex > 0) {
                const Rectangle src = {
                    (float) (tileIndex - 1 % (map->tileset.width / TILE_SIZE)) * TILE_SIZE,
                    (float) (tileIndex / (map->tileset.width / TILE_SIZE)) * TILE_SIZE,
                    TILE_SIZE, TILE_SIZE
                };
                const Vector2 pos = { (float) x * TILE_SIZE, (float) y * TILE_SIZE };
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