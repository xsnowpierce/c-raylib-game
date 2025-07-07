#include "input.h"
#include "raylib.h"

int PLAYER_INPUT_X = 0;
int PLAYER_INPUT_JUMP = 0;
int PLAYER_INPUT_DOWN = 0;
int PLAYER_INPUT_ATTACK = 0;

void ReadInputs() {

    // RESET INPUTS
    PLAYER_INPUT_JUMP = 0;

    bool PLAYER_LEFT = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    bool PLAYER_RIGHT = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

    PLAYER_INPUT_DOWN = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);

    PLAYER_INPUT_ATTACK = IsKeyDown(MOUSE_BUTTON_LEFT) || IsKeyDown(KEY_Z);

    if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_X)) {
        PLAYER_INPUT_JUMP = 1;
    }

    if (PLAYER_LEFT && PLAYER_RIGHT) {
        PLAYER_INPUT_X = 0;
    }
    else {
        if (PLAYER_LEFT) {
            PLAYER_INPUT_X = -1;
        }
        else if (PLAYER_RIGHT) {
            PLAYER_INPUT_X = 1;
        }
        else PLAYER_INPUT_X = 0;
    }
}