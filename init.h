
#ifndef INIT_H
#define INIT_H
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define WIDTH 800
#define HEIGHT WIDTH
#define RECT_DIM 10
#define COLUMNS WIDTH / RECT_DIM
#define ROWS COLUMNS
#define DELAY 100000

#define BACKGROUND BLACK

void init_world_as_glider(bool world[ROWS][COLUMNS])
{
    world[0][1] = true;
    world[1][2] = true;
    world[2][0] = true;
    world[2][1] = true;
    world[2][2] = true;
}

void init_world_as_block(bool world[ROWS][COLUMNS])
{
    world[1][1] = true;
    world[1][2] = true;
    world[2][1] = true;
    world[2][2] = true;
}

void init_world_as_blinker(bool world[ROWS][COLUMNS])
{
    world[1][1] = true;
    world[1][2] = true;
    world[1][3] = true;
}

void init_world_as_pulsar(bool world[ROWS][COLUMNS])
{
    world[0][1] = true;
    world[0][2] = true;
    world[0][6] = true;
    world[0][7] = true;

    world[1][0] = true;
    world[1][3] = true;
    world[1][5] = true;
    world[1][8] = true;

    world[2][0] = true;
    world[2][3] = true;
    world[2][5] = true;
    world[2][8] = true;

    world[3][1] = true;
    world[3][2] = true;
    world[3][6] = true;
    world[3][7] = true;
}

void init_world_as_glider_gun(bool world[ROWS][COLUMNS])
{
    // Initial pattern
    world[5][1] = true;
    world[5][2] = true;
    world[6][1] = true;
    world[6][2] = true;
    world[3][13] = true;
    world[3][14] = true;
    world[4][12] = true;
    world[5][11] = true;
    world[6][11] = true;
    world[7][11] = true;
    world[8][12] = true;
    world[9][13] = true;
    world[9][14] = true;
    world[6][15] = true;
    world[4][16] = true;
    world[8][16] = true;
    world[5][17] = true;
    world[6][17] = true;
    world[7][17] = true;
    world[6][18] = true;

    // Blinker gun
    world[3][21] = true;
    world[4][21] = true;
    world[5][21] = true;
    world[3][22] = true;
    world[4][22] = true;
    world[5][22] = true;
    world[2][23] = true;
    world[6][23] = true;
    world[1][25] = true;
    world[2][25] = true;
    world[6][25] = true;
    world[7][25] = true;

    world[3][35] = true;
    world[4][35] = true;
    world[3][36] = true;
    world[4][36] = true;
}

void init_world_randomly(bool world[ROWS][COLUMNS])
{
    srand(time(0));
    for (size_t i = 0; i < ROWS; ++i){
        for (size_t j = 0; j < COLUMNS; ++j){
            world[i][j] = rand() % 2 == 0;
        }
    }
}

#endif // INIT_H