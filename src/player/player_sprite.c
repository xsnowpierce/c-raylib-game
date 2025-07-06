#include "player_sprite.h"
#include "raylib.h"
#include "../input.h"
#include <string.h>

#include "../defs.h"



Texture2D texture;
Rectangle sourceRect;
Rectangle destRect;
Vector2 origin = { SPRITE_WIDTH / 2.0f, SPRITE_HEIGHT };

float currentSpriteChangeTime = 0.f;
int currentSpriteIndex = 0;

typedef struct SpriteAnimation {
    const float animationSpeed;
    const char* animationFrames;
} SpriteAnimation;

SpriteAnimation idleAnimation = {0.0f, "0"};
SpriteAnimation walkingAnimation = {0.225f, "1020"};
SpriteAnimation jumpingAnimation = {0.0f, "3"};
SpriteAnimation crouchingAnimation = {0.0f, "4"};
SpriteAnimation* currentAnimationData;

enum PLAYER_ANIMATION_STATE currentAnimationState = IDLE;

void InitPlayerAnimation() {
    texture = LoadTexture("resources/penguin.png");
    sourceRect = (Rectangle){ 0.0f, 0.0f, SPRITE_WIDTH, SPRITE_HEIGHT };
    destRect = (Rectangle){ 0.0f, 0.0f, SPRITE_WIDTH, SPRITE_HEIGHT };
    currentAnimationData = &idleAnimation;
}

void UpdateAnimationState(const Player* player) {
    enum PLAYER_ANIMATION_STATE targetAnimationState;

    if (!player->isGrounded) {
        targetAnimationState = JUMPING;
        currentAnimationData = &jumpingAnimation;
    }
    else {
        if (player->isCrouching) {
            targetAnimationState = CROUCHING;
            currentAnimationData = &crouchingAnimation;
        }
        else {
            if (PLAYER_INPUT_X == 0) {
                targetAnimationState = IDLE;
                currentAnimationData = &idleAnimation;
            }
            else {
                targetAnimationState = WALKING;
                currentAnimationData = &walkingAnimation;
            }
        }
    }

    if (targetAnimationState != currentAnimationState) {
        currentAnimationState = targetAnimationState;
        currentSpriteIndex = 0;
        currentSpriteChangeTime = 0.0f;
    }
}

void UpdateSpriteFacingDirection(Player *player) {
    if (PLAYER_INPUT_X != 0) {
        player->facingDirection = PLAYER_INPUT_X;
    }
    sourceRect.width = SPRITE_WIDTH * (player->facingDirection < 0 ? -1 : 1);
}

void UpdatePlayerAnimation(Player *player) {
    UpdateSpriteFacingDirection(player);
    UpdateAnimationState(player);

    currentSpriteChangeTime += GetFrameTime();
    if (currentSpriteChangeTime > currentAnimationData->animationSpeed) {
        currentSpriteIndex++;
        currentSpriteChangeTime = 0.0f;

        if (currentSpriteIndex >= strlen(currentAnimationData->animationFrames)) {
            currentSpriteIndex = 0;
        }
    }

    const int frameValue = currentAnimationData->animationFrames[currentSpriteIndex] - '0';
    sourceRect.x = 16.f * frameValue;

    destRect.x = player->x;
    destRect.y = player->y;
}

void DrawPlayerAnimation(Player *player) {
    DrawTexturePro(texture, sourceRect, destRect, origin, 0.f, WHITE);
    if (SHOW_WORLD_COLLIDERS) {
        DrawRectangleLinesEx(GetPlayerCollider(player), 1, GREEN);
        DrawRectangleLinesEx(GetPlayerHitbox(player), 1, BLUE);
    }
}

void UnloadPlayerAnimation() {
    UnloadTexture(texture);
}
