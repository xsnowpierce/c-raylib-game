#include "player_movement.h"
#include "../level/tilemap.h"
#include "../input.h"
#include "../defs.h"
#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>

#define PLAYER_JUMP_SPEED (-220.0f)
#define PLAYER_GRAVITY 600.0f
#define MAX_JUMP_TIME 0.1f

static float jumpVelocity = 0.0f;
static float currentJumpTime = 0.0f;
static bool isJumping = false;

bool MovePlayer(Player *player, const float dx, const float dy, bool snapToSurface) {
    bool collided = false;
    const CollisionRects collisionRects = GetCollisionRects();
    // get target collider after movement applied
    Rectangle targetRect = GetPlayerCollider(player);
    targetRect.x += dx;
    targetRect.y += dy;

    // check collision against all colliders in the scene
    // this should be probably improved later, only needing to check colliders
    // near the player.
    for (int i = 0; i < collisionRects.count; i++) {
        const Rectangle col = collisionRects.rects[i];
        if (CheckCollisionRecs(targetRect, col)) {
            collided = true;

            if (snapToSurface && dy > 0.0f) {
                float playerBottom = GetPlayerCollider(player).y + GetPlayerCollider(player).height;
                float groundY = col.y;
                float snapAmount = groundY - playerBottom;

                if (snapAmount >= 0.0f && snapAmount <= 2.0f) {
                    player->y += snapAmount;
                    return true;
                }
            }

            return collided;
        }
    }

    player->x += dx;
    player->y += dy;
    return false;
}

void UpdatePlayerMovement(Player *player) {

    const float frameTime = GetFrameTime();

    // X movement
    {
        float moveX = 0.0f;
        if (player->isGrounded) {
            moveX = (float)PLAYER_INPUT_X * frameTime * PLAYER_MOVE_SPEED;
            player->storedJumpDirection = PLAYER_INPUT_X;
        } else {
            moveX = (float)player->storedJumpDirection * frameTime * PLAYER_MOVE_SPEED;
        }
        MovePlayer(player, moveX, 0.0f, false);
    }


    if (PLAYER_INPUT_JUMP && player->isGrounded && !isJumping) {
        isJumping = true;
        currentJumpTime = 0.0f;
        jumpVelocity = PLAYER_JUMP_SPEED;
    }

    currentJumpTime += frameTime;
    if (isJumping && currentJumpTime >= MAX_JUMP_TIME) {
        isJumping = false;
    }

    const float verticalVelocity = jumpVelocity + PLAYER_GRAVITY * currentJumpTime;
    const float moveY = verticalVelocity * frameTime;

    const bool hitGround = MovePlayer(player, 0.0f, moveY, true);

    if (hitGround) {
        isJumping = false;
        jumpVelocity = 0.0f;
        currentJumpTime = 0.0f;
    }

    player->isGrounded = hitGround;
}
