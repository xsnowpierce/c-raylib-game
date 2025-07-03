#include "player.h"

#include <stdlib.h>

#include "defs.h"
#include "input.h"
#include "raylib.h"

Texture2D texture;
Rectangle sourceRect;
Rectangle destRect;
Vector2 origin;
float TIME_UNTIL_SPRITE_CHANGE = 0.225f;
float current_sprite_change_time = 0.f;

enum PLAYER_ANIMATION_STATE {
    IDLE, WALKING, JUMPING
};

enum PLAYER_ANIMATION_STATE currentAnimationState;

void InitPlayer(Player *player) {
    player->x = GAME_SCREEN_WIDTH / 2.0f + 8;
    player->y = GAME_SCREEN_HEIGHT - 16;
    texture = LoadTexture("resources/penguin.png");

    sourceRect.width = 16.f;
    sourceRect.height = 32.f;

    destRect.width = 16.f;
    destRect.height = 32.f;

    sourceRect.x = 0.0f;
    sourceRect.y = 0.0f;

    origin.x = 16.f;
    origin.y = 32.f;

    currentAnimationState = IDLE;
}

void UpdateSpriteFacingDirection(Player *player) {
    if (PLAYER_INPUT_X != 0) {
        player->facingDirection = PLAYER_INPUT_X;
    }

    if (player->facingDirection < 0) {
        sourceRect.width = (float) -(abs(sourceRect.width));
    }
    else if (player->facingDirection > 0) {
        sourceRect.width = (float) abs(sourceRect.width);
    }
}

void UpdateAnimationState() {
    if (PLAYER_INPUT_X == 0) {
        currentAnimationState = IDLE;
    } else {
        currentAnimationState = WALKING;
    }
}

void UpdateSprite(Player *player) {
    UpdateSpriteFacingDirection(player);
    UpdateAnimationState();

    switch (currentAnimationState) {
        // ReSharper disable once CppDFAUnreachableCode
        case IDLE:
            current_sprite_change_time = 0.0f;
            sourceRect.x = 0.f;
            break;
            case WALKING:
            current_sprite_change_time += GetFrameTime();
            if (current_sprite_change_time > TIME_UNTIL_SPRITE_CHANGE) {
                if (sourceRect.x + 16.f >= (float) texture.width) {
                    sourceRect.x = 0;
                }
                else sourceRect.x += 16.f;
                current_sprite_change_time = 0.0f;
            }
            break;
        default:
            break;
    }
}

void UpdatePlayer(Player *player) {
    destRect.x = player->x;
    destRect.y = player->y;

    player->x += (float) PLAYER_INPUT_X * GetFrameTime() * PLAYER_MOVE_SPEED;

    UpdateSprite(player);
}

void DrawPlayer(Player player) {
    DrawTexturePro(texture, sourceRect, destRect, origin, 0.f, WHITE);
}

void UnloadPlayer(Player *player) {
    UnloadTexture(texture);
}