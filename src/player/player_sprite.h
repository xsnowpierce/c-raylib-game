#ifndef PLAYER_SPRITE_H
#define PLAYER_SPRITE_H

#include "player.h"

void InitPlayerAnimation();
void UpdatePlayerAnimation(Player *player);
void DrawPlayerAnimation(Player *player);
void UnloadPlayerAnimation();

#endif
