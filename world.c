#include "world.h"
#include "raylib.h"

Texture2D mapImage;
Rectangle spriteRect;

void CreateWorld() {
    mapImage = LoadTexture("resources/map.png");
}

void DrawWorld() {
    DrawTexture(mapImage, 0, 0, WHITE);
}

void UnloadWorld() {
    UnloadTexture(mapImage);
}
