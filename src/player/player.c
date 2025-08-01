#include "player.h"

#include <raylib.h>
#include <stdbool.h>

#include "player_combat.h"
#include "../defs.h"
#include "player_movement.h"
#include "player_sprite.h"

void InitPlayer(Player *player) {
    player->x = GAME_SCREEN_WIDTH / 2.0f + 8;
    player->y = GAME_SCREEN_HEIGHT - 144;
    player->facingDirection = 1;
    player->isGrounded = false;
    player->isCrouching = false;
    player->storedJumpDirection = 0;

    InitPlayerAnimation();
}

void UpdatePlayer(Player *player) {
    UpdatePlayerMovement(player);
    UpdatePlayerCombat(player);
    UpdatePlayerAnimation(player);
}

void DrawPlayer(Player *player) {
    DrawPlayerAnimation(player);
}

void UnloadPlayer(Player *player) {
    UnloadPlayerAnimation();
}

Rectangle GetPlayerCollider(const Player *player) {
    return (Rectangle){
        player->x - SPRITE_WIDTH / 2,
        player->y - SPRITE_HEIGHT,
        SPRITE_WIDTH - 1,
        SPRITE_HEIGHT
    };
}

Rectangle GetPlayerHitbox(const Player *player) {

    // CROUCHING HITBOX
    if (player->isCrouching && player->isGrounded) {
        return (Rectangle){
            player->x - SPRITE_WIDTH / 2,
            player->y - SPRITE_HEIGHT + 12,
            SPRITE_WIDTH - 1,
            20
        };
    }

    // STANDARD HITBOX
    return (Rectangle){
        player->x - SPRITE_WIDTH / 2,
        player->y - SPRITE_HEIGHT,
        SPRITE_WIDTH - 1,
        SPRITE_HEIGHT
    };
}
