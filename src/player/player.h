#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <stdbool.h>

typedef struct Player {
    float x, y;
    int facingDirection;
    bool isGrounded;
    int storedJumpDirection;
} Player;

void InitPlayer(Player *player);
void UpdatePlayer(Player *player);
void DrawPlayer(Player *player);
void UnloadPlayer(Player *player);
Rectangle GetPlayerCollider(Player *player);

#endif
