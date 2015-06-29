#ifndef GAME_H
#define GAME_H

#define GMK_UP 1
#define GMK_DOWN 2
#define GMK_LEFT 4
#define GMK_RIGHT 8

typedef struct {
    int score;
    float zoom;

    float ppw;
    float pph;

    unsigned keysDown;
} GameInfo;

extern GameInfo glob_game;

void gm_start();

#endif
