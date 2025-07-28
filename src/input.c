#include "input.h"
#include "raylib.h"

int PLAYER_INPUT_X = 0;
bool PLAYER_INPUT_LEFT = false;
bool PLAYER_INPUT_RIGHT = false;
bool PLAYER_INPUT_JUMP = false;
bool PLAYER_INPUT_DOWN = false;
bool PLAYER_INPUT_ATTACK = false;

void ReadInputs() {

    // RESET INPUTS
    PLAYER_INPUT_JUMP = 0;

    PLAYER_INPUT_LEFT = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    PLAYER_INPUT_RIGHT = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

    PLAYER_INPUT_DOWN = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);

    PLAYER_INPUT_ATTACK = IsKeyDown(MOUSE_BUTTON_LEFT) || IsKeyDown(KEY_Z);

    if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_X)) {
        PLAYER_INPUT_JUMP = 1;
    }

    if (PLAYER_INPUT_LEFT && PLAYER_INPUT_RIGHT) {
        PLAYER_INPUT_LEFT = false;
        PLAYER_INPUT_RIGHT = false;
    }
    else {
        if (PLAYER_INPUT_LEFT) {
            PLAYER_INPUT_X = -1;
        }
        else if (PLAYER_INPUT_RIGHT) {
            PLAYER_INPUT_X = 1;
        }
        else PLAYER_INPUT_X = 0;
    }
}