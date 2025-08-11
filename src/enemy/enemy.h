#pragma once

#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_ARRAY_SIZE 1

typedef enum ENEMY_TYPE{
    BAT
} ENEMY_TYPE;

typedef struct Enemy{

    float positionX, positionY;
    ENEMY_TYPE type;
    int health;
    float rectangleWidth, rectangleHeight;

} Enemy;


extern Enemy enemies[ENEMY_ARRAY_SIZE];

void InitEnemies();
void UpdateEnemies();
void DrawEnemies();

#endif