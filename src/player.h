//
// Created by snow on 01/07/25.
//

#pragma once

typedef struct Player {
    float x, y;
    float vy;
    int isGrounded;
    int facingDirection;
} Player;

void InitPlayer(Player *player);
void UpdatePlayer(Player *player);
void DrawPlayer(Player *player);
void UnloadPlayer(Player *player);

struct Rectangle; // forward declare for return type
struct Rectangle GetPlayerCollider(Player *player);
