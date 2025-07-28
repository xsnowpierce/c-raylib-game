//
// Created by snow on 07/07/25.
//

#ifndef SPRITE_H
#define SPRITE_H

#define MAX_SPRITES 5

typedef struct {
    int spriteX, spriteY;
    int spriteWidth, spriteHeight;
    int spriteOffsetX, spriteOffsetY;
    float spriteAnimationDelay;
} Sprite;

typedef struct {
    float animationSpeed;
    Sprite rightSprites[MAX_SPRITES];
    Sprite leftSprites[MAX_SPRITES];
    int spriteCount;
} SpriteAnimation;

#endif //SPRITE_H
