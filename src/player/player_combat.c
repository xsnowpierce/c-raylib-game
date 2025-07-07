//
// Created by snow on 07/07/25.
//
#include "player_combat.h"

#include <stdio.h>

#include "../input.h"

void UpdatePlayerCombat(Player *player) {
    //printf("player is attacking: %d\n", player->isAttacking);
    if (!player->isAttacking) {
        if (PLAYER_INPUT_ATTACK) {
            player->isAttacking = true;
        }
    }
}
