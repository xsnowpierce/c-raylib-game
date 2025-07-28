//
// Created by snow on 07/07/25.
//
#include "player_combat.h"

#include "../input.h"
#include <stdbool.h>

bool ATTACK_INPUT_LAST_FRAME = false;

void UpdatePlayerCombat(Player *player) {

    if (!player->isAttacking) {
        if (PLAYER_INPUT_ATTACK && ATTACK_INPUT_LAST_FRAME == false) {
            player->isAttacking = true;
        }
    }

    ATTACK_INPUT_LAST_FRAME = PLAYER_INPUT_ATTACK;
}
