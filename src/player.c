#include "player.h"

#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "input.h"
#include "raylib.h"
#include "tilemap.h"

#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 32

// SPRITE VARS
Texture2D texture;
Rectangle sourceRect;
Rectangle destRect;
Vector2 origin = (Vector2) {SPRITE_WIDTH, SPRITE_HEIGHT};

// SPRITE ANIMATION VARS
float currentSpriteChangeTime = 0.f;
int currentSpriteIndex = 0;
typedef struct SpriteAnimation {
    const float animationSpeed;
    const char* animationFrames;
} SpriteAnimation;

SpriteAnimation walkingAnimation = {0.225f, "1020"};
SpriteAnimation idleAnimation = {0.0f, "0" };
SpriteAnimation* currentAnimationData;

// PLAYER STATE VARS
enum PLAYER_ANIMATION_STATE {
    IDLE = 0, WALKING = 1, JUMPING = 2
};
enum PLAYER_ANIMATION_STATE currentAnimationState;

void InitPlayer(Player *player) {
    player->x = GAME_SCREEN_WIDTH / 2.0f + 8;
    player->y = GAME_SCREEN_HEIGHT - 16;
    texture = LoadTexture("resources/penguin.png");

    sourceRect = (Rectangle){ 0.0f, 0.0f, SPRITE_WIDTH, SPRITE_HEIGHT };
    destRect = (Rectangle){ player->x, player->y, SPRITE_WIDTH, SPRITE_HEIGHT };

    currentAnimationState = IDLE;
    currentAnimationData = &idleAnimation;
}

void UpdateSpriteFacingDirection(Player *player) {
    if (PLAYER_INPUT_X != 0) {
        player->facingDirection = PLAYER_INPUT_X;
    }
    // flip sprite if going other way (make it negative so that its backwards)
    sourceRect.width = SPRITE_WIDTH * (player->facingDirection < 0 ? -1 : 1);
}

void UpdateAnimationState() {
    enum PLAYER_ANIMATION_STATE targetAnimationState;

    if (PLAYER_INPUT_X == 0) {
        targetAnimationState = IDLE;
        currentAnimationData = &idleAnimation;
    } else {
        targetAnimationState = WALKING;
        currentAnimationData = &walkingAnimation;
    }

    if (targetAnimationState != currentAnimationState) {
        // animation state change
        currentAnimationState = targetAnimationState;
        currentSpriteIndex = 0;
        currentSpriteChangeTime = 0.0f;
    }
}

void UpdateSprite(Player *player) {
    UpdateSpriteFacingDirection(player);
    UpdateAnimationState();

    const float currentAnimationSpeed = currentAnimationData->animationSpeed;
    const unsigned long currentAnimationLength = strlen(currentAnimationData->animationFrames);

    currentSpriteChangeTime += GetFrameTime();
    if (currentSpriteChangeTime > currentAnimationSpeed) {

        currentSpriteIndex++;
        currentSpriteChangeTime = 0.0f;

        if (currentSpriteIndex >= currentAnimationLength) {
            //printf("animation was frame %d, changing it to %d\n", currentSpriteIndex, 0);
            currentSpriteIndex = 0;
        }
    }

    const int frameValue = currentAnimationData->animationFrames[currentSpriteIndex] - '0';
    sourceRect.x = 16 * frameValue;
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