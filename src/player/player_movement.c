#include "player_movement.h"
#include "../level/tilemap.h"
#include "../input.h"
#include "../defs.h"
#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define PLAYER_MOVE_SPEED 80
#define PLAYER_JUMP_SPEED (-220.0f)
#define PLAYER_GRAVITY 600.0f
#define MAX_JUMP_TIME 0.1f
#define PLAYER_JUMP_RECOVERY_TIME 0.25f

static float jumpVelocity = 0.0f;
static float currentJumpTime = 0.0f;
static bool isJumping = false;
bool PLAYER_JUMPED_THIS_INPUT = false;
float postJumpRecoveryTime = 0.0f;

bool MovePlayer(Player *player, const float dx, const float dy) {
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

            // todo
            // add snapping so that the player will be 0.0f away from the wall
            // they are colliding with.
            // this will make it so that you can jump in a 2x wide area
            // since it would otherwise require exact pixel perfect positioning


            // it has been band-aid fixed by putting the player collider 1 pixel thinner,
            // but this makes it so that the player sprite overlaps walls by a pixel
            return collided;
        }
    }

    player->x += dx;
    player->y += dy;
    return false;
}

void UpdatePlayerMovement(Player *player) {

    const float frameTime = GetFrameTime();

    // INPUT
    if (!PLAYER_INPUT_JUMP) {
        PLAYER_JUMPED_THIS_INPUT = false;
    }

    if (player->isGrounded) {
        if (postJumpRecoveryTime > 0.0f) {
            postJumpRecoveryTime -= frameTime;
        }

        if (PLAYER_INPUT_DOWN) {
            player->isCrouching = true;
        }
        else player->isCrouching = false;


    }

    // X movement
    if (!player->isCrouching) {
        float moveX = 0.0f;
        if (player->isGrounded) {
            moveX = (float)PLAYER_INPUT_X * frameTime * PLAYER_MOVE_SPEED;
            player->storedJumpDirection = PLAYER_INPUT_X;
        } else {
            moveX = (float)player->storedJumpDirection * frameTime * PLAYER_MOVE_SPEED;
        }
        MovePlayer(player, moveX, 0.0f);
    }

    // JUMP movement
    if (PLAYER_INPUT_JUMP && player->isGrounded && !player->isCrouching && !isJumping && !PLAYER_JUMPED_THIS_INPUT && postJumpRecoveryTime <= 0.f) {
        PLAYER_JUMPED_THIS_INPUT = true;
        isJumping = true;
        currentJumpTime = 0.0f;
        postJumpRecoveryTime = PLAYER_JUMP_RECOVERY_TIME;
        jumpVelocity = PLAYER_JUMP_SPEED;
    }

    currentJumpTime += frameTime;
    if (isJumping && currentJumpTime >= MAX_JUMP_TIME) {
        isJumping = false;
    }

    // Y movement
    const float verticalVelocity = jumpVelocity + PLAYER_GRAVITY * currentJumpTime;
    const float moveY = verticalVelocity * frameTime;

    const bool hitGround = MovePlayer(player, 0.0f, moveY);

    if (hitGround) {
        isJumping = false;
        jumpVelocity = 0.0f;
        currentJumpTime = 0.0f;
    }
    Rectangle targetRect = GetPlayerCollider(player);
    targetRect.y += 1;

    // COLLISION detection
    const CollisionRects collisionRects = GetCollisionRects();
    bool is_grounded = false;
    for (int i = 0; i < collisionRects.count; i++) {
        if (CheckCollisionRecs(targetRect, collisionRects.rects[i])) {
            is_grounded = true;
            break;
        }
    }

    player->isGrounded = is_grounded;
}