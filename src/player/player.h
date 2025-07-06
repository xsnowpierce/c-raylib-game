#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <stdbool.h>

#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 32

typedef struct Player {
    float x, y;
    float velocityY;
    int storedJumpDirection;
    int facingDirection;
    bool isGrounded;
} Player;


void InitPlayer(Player *player);
void UpdatePlayer(Player *player);
void DrawPlayer(Player *player);
void UnloadPlayer(Player *player);
Rectangle GetPlayerCollider(const Player *player);

#endif
