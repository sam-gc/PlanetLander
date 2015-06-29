#ifndef GAME_H
#define GAME_H

typedef struct {
    int score;
    float zoom;

    float ppw;
    float pph;
} GameInfo;

extern GameInfo glob_game;

void gm_start();

#endif
