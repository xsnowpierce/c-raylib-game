#include "enemy.h"
#include "raylib.h"x

Enemy enemies[ENEMY_ARRAY_SIZE];
Texture2D enemyTexture;


Vector2 GetEnemyPosition(int enemyFromArray){
    return (Vector2) {enemies[enemyFromArray].positionX, enemies[enemyFromArray].positionY };
}

void SetEnemyPosition(int enemyFromArray, Vector2 position){
    if(enemyFromArray < 0 || enemyFromArray > ENEMY_ARRAY_SIZE){
        return;
    }
    
    enemies[enemyFromArray].positionX = position.x;
    enemies[enemyFromArray].positionY = position.y;
}

void MoveEnemy(int enemyFromArray, Vector2 amount){
    SetEnemyPosition(enemyFromArray, (Vector2) {GetEnemyPosition(enemyFromArray).x + amount.x, GetEnemyPosition(enemyFromArray).y + amount.y});
}

void SetEnemyHealth(int enemyFromArray, int health) {
    if(enemyFromArray < 0 || enemyFromArray > ENEMY_ARRAY_SIZE){
        return;
    }

    enemies[enemyFromArray].health = health;
}

void InitEnemies(){
    enemyTexture = LoadTexture("resources/enemies.png");

    for (int i = 0; i < ENEMY_ARRAY_SIZE; i++){
        SetEnemyPosition(i, (Vector2) {i * 5, 0});
        SetEnemyHealth(i, 10);
        enemies[i].type = BAT;
        enemies[i].rectangleWidth = 16;
        enemies[i].rectangleHeight = 32;
    }
}

void UpdateEnemies(){
    const float frameTime = GetFrameTime();
    const float moveSpeed = 2;

    for(int i = 0; i < ENEMY_ARRAY_SIZE; i++){
        MoveEnemy(i, (Vector2) {2 * frameTime * moveSpeed, 0});
    }
}

void DrawEnemies(){
    for (int i = 0; i < ENEMY_ARRAY_SIZE; i++){
        DrawTexturePro(enemyTexture, (Rectangle) {16, 0, 16, 32}, (Rectangle) {GetEnemyPosition(i).x, GetEnemyPosition(i).y, 16, 32}, (Vector2) {}, 0, WHITE);
    }
}