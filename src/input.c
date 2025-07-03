#include "input.h"
#include "raylib.h"

int PLAYER_INPUT_X = 0;

void ReadInputs() {
    bool PLAYER_LEFT = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);
    bool PLAYER_RIGHT = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

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