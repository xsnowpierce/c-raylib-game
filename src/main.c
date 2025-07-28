#include "defs.h"
#include "raylib.h"
#include "input.h"
#include "player/player.h"
#include "level/world.h"

int PLAYER_SCORE;
int PLAYER_TIME;
int PLAYER_STAGE;

int main(void) {
    const int SCREEN_WIDTH = GAME_SCREEN_WIDTH * SCREEN_SIZE_MULTIPLIER;
    const int SCREEN_HEIGHT = GAME_SCREEN_HEIGHT * SCREEN_SIZE_MULTIPLIER;
    PLAYER_SCORE = 0;
    PLAYER_TIME = 250;
    PLAYER_STAGE = 1;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
    SetConfigFlags(FLAG_VSYNC_HINT);


    Player player;
    InitPlayer(&player);
    CreateWorld();

    Camera2D camera;
    camera.target = (Vector2) {player.x, GAME_SCREEN_HEIGHT - 128 - 8};
    camera.offset = (Vector2) {GAME_SCREEN_WIDTH * SCREEN_SIZE_MULTIPLIER / 2.0f, (GAME_SCREEN_HEIGHT - 24) * SCREEN_SIZE_MULTIPLIER};
    camera.rotation = 0;
    camera.zoom = SCREEN_SIZE_MULTIPLIER;

    const Font font = LoadFontEx("resources/haunted-castle.ttf", 8, 0, 74);

    while (!WindowShouldClose()) {
        ReadInputs();
        camera.target = (Vector2) {player.x - 8, camera.target.y};
        UpdatePlayer(&player);
        //printf("player is grounded: %d\n", player.isGrounded);

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode2D(camera);

        DrawWorld();
        DrawPlayer(&player);

        EndMode2D();

        // draw UI boxes
        DrawRectangle(0, 0, SCREEN_WIDTH, 32 * SCREEN_SIZE_MULTIPLIER, BLACK);
        DrawRectangle(0, SCREEN_HEIGHT - (16 * SCREEN_SIZE_MULTIPLIER), SCREEN_WIDTH, 16 * SCREEN_SIZE_MULTIPLIER, BLACK);

        // draw UI text
        DrawTextEx(font, TextFormat("SCORE-%06d TIME %04d STAGE %02d", PLAYER_SCORE, PLAYER_TIME, PLAYER_STAGE), (Vector2) {0, 0}, 8 * SCREEN_SIZE_MULTIPLIER, 0, WHITE);

        EndDrawing();
    }

    UnloadPlayer(&player);
    UnloadWorld();
    CloseWindow();
    return 0;
}
