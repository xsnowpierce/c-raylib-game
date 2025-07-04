#include "player.h"

#include <raylib.h>
#include <stdbool.h>

#include "../defs.h"
#include "player_movement.h"
#include "player_sprite.h"

void InitPlayer(Player *player) {
    player->x = GAME_SCREEN_WIDTH / 2.0f + 8;
    player->y = GAME_SCREEN_HEIGHT - 32;
    player->facingDirection = 1;
    player->isGrounded = false;
    player->storedJumpDirection = 0;

    InitPlayerAnimation();
}

void UpdatePlayer(Player *player) {
    UpdatePlayerMovement(player);
    UpdatePlayerAnimation(player);
}

void DrawPlayer(Player *player) {
    DrawPlayerAnimation(player);
}

void UnloadPlayer(Player *player) {
    UnloadPlayerAnimation();
}

Rectangle GetPlayerCollider(Player *player) {
    extern Rectangle destRect;
    return (Rectangle){ destRect.x - 16, destRect.y - 32, 16, 32 };
}
