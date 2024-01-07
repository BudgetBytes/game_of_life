// https://it.wikipedia.org/wiki/Gioco_della_vita

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "raylib.h"

#define COLUMNS 600
#define ROWS COLUMNS
#define RECT_DIM 10
#define DELAY 10000


void init_world_as_glider(bool world[ROWS][COLUMNS])
{
    world[0][1] = true;
    world[1][2] = true;
    world[2][0] = true;
    world[2][1] = true;
    world[2][2] = true;
}

void init_world_as_block(bool world[ROWS][COLUMNS]) {
    world[1][1] = true;
    world[1][2] = true;
    world[2][1] = true;
    world[2][2] = true;
}

void init_world_as_blinker(bool world[ROWS][COLUMNS]) {
    world[1][1] = true;
    world[1][2] = true;
    world[1][3] = true;
}

void init_world_as_glider_gun(bool world[ROWS][COLUMNS]) {
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


void init_world_randomly(bool world[ROWS][COLUMNS]) {
    srand(time(0));
    for (size_t i = 0; i < ROWS; ++i) {
        for (size_t j = 0; j < COLUMNS; ++j) {
            if (rand() % 2 == 0) {
                world[i][j] = true;
            }
        }
    }
}


int count_live_neighbors(bool world[ROWS][COLUMNS], int row, int col) {
    int count = 0;
    for (int i = row - 1; i <= row + 1; ++i) {
        for (int j = col - 1; j <= col + 1; ++j) {
            if (i >= 0 && i < ROWS && j >= 0 && j < COLUMNS && !(i == row && j == col) && world[i][j]) {
                count++;
            }
        }
    }
    return count;
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 800;
    bool world[ROWS][COLUMNS] = {false};
    init_world_randomly(world);

    InitWindow(screenWidth, screenHeight, "Game of Life");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            bool newWorld[ROWS][COLUMNS] = {false};
            for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLUMNS; ++j) {
                    int liveNeighbors = count_live_neighbors(world, i, j);

                    if (world[i][j]) {
                        if (liveNeighbors == 2 || liveNeighbors == 3) {
                            newWorld[i][j] = true;
                        } else if(liveNeighbors < 2 || liveNeighbors > 3) {
                            newWorld[i][j] = false;
                        }
                    } else {
                        if (liveNeighbors == 3) {
                            newWorld[i][j] = true;
                        }
                    }

                    if (newWorld[i][j]) {
                        DrawRectangle(screenWidth / COLUMNS + j * RECT_DIM, screenHeight / ROWS + i * RECT_DIM, RECT_DIM, RECT_DIM, RAYWHITE);
                    }
                }
            }

            for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLUMNS; ++j) {
                    world[i][j] = newWorld[i][j];
                }
            }
            usleep(DELAY);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}