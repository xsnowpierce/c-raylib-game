#ifndef PLAYER_SPRITE_H
#define PLAYER_SPRITE_H

#include "player.h"

void InitPlayerAnimation();
void UpdatePlayerAnimation(Player *player);
void DrawPlayerAnimation(Player *player);
void UnloadPlayerAnimation();

enum PLAYER_ANIMATION_STATE { IDLE, WALKING, JUMPING, CROUCHING, STAND_ATTACKING, CROUCH_ATTACKING };
extern enum PLAYER_ANIMATION_STATE currentAnimationState;

#endif
