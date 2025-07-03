//
// Created by snow on 01/07/25.
//

#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player {
    float x, y;
    int facingDirection;

} Player;

void InitPlayer(Player *player);
void UpdatePlayer(Player *player);
void DrawPlayer(Player player);
void UnloadPlayer(Player *player);

#endif //PLAYER_H
