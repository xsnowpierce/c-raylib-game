#pragma once

#include "raylib.h"

#define TILE_SIZE 8
#define MAX_MAP_WIDTH 32 * TILE_SIZE
#define MAX_MAP_HEIGHT 30 * TILE_SIZE

typedef struct {
    int *tiles;
    int width;
    int height;
} TileLayer;

typedef struct {
    Texture2D tileset;
    TileLayer groundLayer;
    TileLayer collisionLayer;
} TileMap;

typedef struct {
    Rectangle *rects;
    int count;
    int capacity;
} CollisionRects;



void LoadTileMap(TileMap *map, const char *mapFile, const char *tilesetFile);
void DrawTileMap(TileMap *map);
void DrawDebugCollisionRects();
//bool IsTileSolidAt(TileMap *map, int tileX, int tileY);
