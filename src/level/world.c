#include "world.h"

#include "../defs.h"
#include "tilemap.h"

TileMap worldMap;

void CreateWorld() {
    LoadTileMap(&worldMap, "tiled/map.json", "tiled/tileset.png");
}

void DrawWorld() {
    DrawTileMap(&worldMap);

    if (SHOW_WORLD_COLLIDERS) {
        DrawDebugCollisionRects();
    }
}

void UnloadWorld() {
    UnloadTexture(worldMap.tileset);
}

