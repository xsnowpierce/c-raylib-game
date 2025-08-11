#include "player_sprite.h"

#include "raylib.h"
#include "../input.h"

#include "../defs.h"
#include "../sprite/sprite.h"



Texture2D texture;
Rectangle sourceRect;
Rectangle destRect;
Vector2 origin = { SPRITE_WIDTH / 2.0f, SPRITE_HEIGHT };

float currentSpriteChangeTime = 0.f;
float currentSpriteDelayTime = 0.f;
int currentSpriteIndex = 0;

SpriteAnimation idleAnimation = {
    .animationSpeed = 0.0f,
    .rightSprites = {
        {.spriteX = 0, .spriteY = 0, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT }
    },
    .leftSprites = {
        {.spriteX = 0, .spriteY = 32, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT }
    },
    .spriteCount = 1
};

SpriteAnimation walkingAnimation = {
    .animationSpeed = 0.225f,
    .rightSprites = {
        {.spriteX = 16, .spriteY = 0, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT },
        {.spriteX = 0, .spriteY = 0, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT },
        {.spriteX = 32, .spriteY = 0, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT },
        {.spriteX = 0, .spriteY = 0, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT }
    },
    .leftSprites = {
        {.spriteX = 16, .spriteY = 32, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT },
        {.spriteX = 0, .spriteY = 32, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT },
        {.spriteX = 32, .spriteY = 32, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT },
        {.spriteX = 0, .spriteY = 32, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT }
    },
    .spriteCount = 4
};

SpriteAnimation jumpingAnimation = {
    .animationSpeed = 0.0f,
    .rightSprites = {
        {.spriteX = 32, .spriteY = 0, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT }
    },
    .leftSprites = {
        {.spriteX = 32, .spriteY = 32, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT }
    },
    .spriteCount = 1
};

SpriteAnimation crouchingAnimation = {
    .animationSpeed = 0.0f,
    .rightSprites = {
        {.spriteX = 48, .spriteY = 0, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT }
    },
    .leftSprites = {
        {.spriteX = 48, .spriteY = 32, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT }
    },
    .spriteCount = 1
};

#define PLAYER_SWORD_HOLD_TIME 0.2f

SpriteAnimation standingAttackAnimation = {
    .animationSpeed = .07f,
    .rightSprites = {
        {.spriteX = 0, .spriteY = 0, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT },
        {.spriteX = 16, .spriteY = 64, .spriteWidth = SPRITE_WIDTH + 1, .spriteHeight = SPRITE_HEIGHT},
        {.spriteX = 64, .spriteY = 64, .spriteWidth = SPRITE_WIDTH + 27, .spriteHeight = SPRITE_HEIGHT, .spriteAnimationDelay = PLAYER_SWORD_HOLD_TIME}
    },
    .leftSprites = {
        {.spriteX = 0, .spriteY = 32, .spriteWidth = SPRITE_WIDTH, .spriteHeight = SPRITE_HEIGHT },
        {.spriteX = 16, .spriteY = 96, .spriteWidth = SPRITE_WIDTH + 1, .spriteHeight = SPRITE_HEIGHT},
        {.spriteX = 37, .spriteY = 96, .spriteOffsetX = -27, .spriteWidth = SPRITE_WIDTH + 27, .spriteHeight = SPRITE_HEIGHT, .spriteAnimationDelay = PLAYER_SWORD_HOLD_TIME}
    },
    .spriteCount = 3
};

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

    if (player->isAttacking) {
        if (player->isCrouching) {
            targetAnimationState = CROUCH_ATTACKING;
            // todo: update this to crouch attack animation
            currentAnimationData = &crouchingAnimation;
        }
        else {
            targetAnimationState = STAND_ATTACKING;
            currentAnimationData = &standingAttackAnimation;
        }
    }

    else if (!player->isGrounded) {
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

    //printf("player animation state: %d\n", currentAnimationState);
}

void UpdatePlayerAnimation(Player *player) {

    // update playerFacingDirection
    if (PLAYER_INPUT_X != 0 && !player->isAttacking) {
        player->facingDirection = PLAYER_INPUT_X;
    }

    // update currentAnimationData
    UpdateAnimationState(player);


    // create potential sprite offset
    int spriteXOffset = 0;

    // determine which sprite set we should use
    const Sprite* currentSprites = (player->facingDirection == -1) ? currentAnimationData->leftSprites : currentAnimationData->rightSprites;

    // update sprite delay time
    if (currentSpriteDelayTime > 0.0f) {
        currentSpriteDelayTime -= GetFrameTime();
    }
    else {
        

        // sprite animating iterator
        currentSpriteChangeTime += GetFrameTime(); // timer
        if (currentSpriteChangeTime > currentAnimationData->animationSpeed) {
            // timer reached time to change sprite
            currentSpriteIndex++;
            currentSpriteChangeTime = 0.0f;
            currentSpriteDelayTime = currentSprites[currentSpriteIndex].spriteAnimationDelay;

            if (currentSpriteIndex >= currentAnimationData->spriteCount) {

                // check if its the end of an attack animation, if so, then set attacking to false
                if (currentAnimationData == &standingAttackAnimation) {
                    player->isAttacking = false;
                }

                // reached end of array, reset to 0
                currentSpriteIndex = 0;
            }
        }

        // Update sprite rect
        sourceRect = (Rectangle) {currentSprites[currentSpriteIndex].spriteX, currentSprites[currentSpriteIndex].spriteY, currentSprites[currentSpriteIndex].spriteWidth, currentSprites[currentSpriteIndex].spriteHeight}; // NOLINT(*-narrowing-conversions)
        
        destRect.width = currentSprites[currentSpriteIndex].spriteWidth;
        destRect.height = currentSprites[currentSpriteIndex].spriteHeight;

        
    }

    spriteXOffset = currentSprites[currentSpriteIndex].spriteOffsetX;

    // just make sure the Dest Rect is at the position of the player
    destRect.x = player->x + spriteXOffset;
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
