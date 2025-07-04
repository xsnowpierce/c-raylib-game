#include "player_movement.h"
#include "../level/tilemap.h"
#include "../input.h"
#include "../defs.h"
#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>

#define PLAYER_JUMP_SPEED -220.0f
#define PLAYER_GRAVITY 400.0f
#define MAX_JUMP_TIME 0.25f

static float jumpVelocity = 0.0f;
static float currentJumpTime = 0.0f;
static bool isJumping = false;

void TryMove(Player *player, float dx, float dy, const CollisionRects *collisionRects, bool *collided, bool snapToSurface) {
    Rectangle targetRect = GetPlayerCollider(player);
    targetRect.x += dx;
    targetRect.y += dy;

    for (int i = 0; i < collisionRects->count; i++) {
        Rectangle col = collisionRects->rects[i];
        if (CheckCollisionRecs(targetRect, col)) {
            if (collided) *collided = true;

            if (snapToSurface && dy > 0.0f) {
                float snapAmount = col.y - (GetPlayerCollider(player).y + GetPlayerCollider(player).height);
                player->y += snapAmount;
            }
            return;
        }
    }

    player->x += dx;
    player->y += dy;
    if (collided) *collided = false;
}

void UpdatePlayerMovement(Player *player) {
    const CollisionRects collisionRects = GetCollisionRects();
    float frameTime = GetFrameTime();

    float moveX = 0.0f;
    if (player->isGrounded) {
        moveX = (float)PLAYER_INPUT_X * frameTime * PLAYER_MOVE_SPEED;
        player->storedJumpDirection = PLAYER_INPUT_X;
    } else {
        moveX = (float)player->storedJumpDirection * frameTime * PLAYER_MOVE_SPEED;
    }

    TryMove(player, moveX, 0.0f, &collisionRects, NULL, false);

    if (PLAYER_INPUT_JUMP && player->isGrounded && !isJumping) {
        isJumping = true;
        currentJumpTime = 0.0f;
        jumpVelocity = PLAYER_JUMP_SPEED;
    }

    currentJumpTime += frameTime;
    if (isJumping && currentJumpTime >= MAX_JUMP_TIME) {
        isJumping = false;
    }

    float verticalVelocity = jumpVelocity + PLAYER_GRAVITY * currentJumpTime;
    float moveY = verticalVelocity * frameTime;

    bool hitGround = false;
    TryMove(player, 0.0f, moveY, &collisionRects, &hitGround, true);

    if (hitGround) {
        isJumping = false;
        jumpVelocity = 0.0f;
        currentJumpTime = 0.0f;
    }

    player->isGrounded = hitGround;
}
