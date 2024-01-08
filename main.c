// https://it.wikipedia.org/wiki/Gioco_della_vita

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "raylib.h"

#define WIDTH 800
#define HEIGHT WIDTH
#define COLUMNS 100
#define ROWS COLUMNS
#define RECT_DIM (WIDTH / COLUMNS - 1) - 1 // don't know why but if i set -1 after it gives rectangle 1px padding
#define DELAY 100000

const Color COLORS[] = {GRAY,GOLD,PINK,RED,LIME,SKYBLUE,VIOLET,BEIGE};
#define COLORS_LEN 8

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

void init_world_as_pulsar(bool world[ROWS][COLUMNS]) {
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

int count_live_neighbors(bool world[ROWS][COLUMNS], int row, int col)
{
    int count = 0;
    for (int i = row - 1; i <= row + 1; ++i){
        for (int j = col - 1; j <= col + 1; ++j){
            if (i >= 0 && i < ROWS && j >= 0 && j < COLUMNS && !(i == row && j == col) && world[i][j]){
                count++;
            }
        }
    }
    return count;
}

char *shift_args(int *argc, char ***argv) {
    if (*argc < 1 ) {
        return NULL;
    }
    char *arg = malloc(strlen(**argv) + 1); 
    strcpy(arg, **argv);

    (*argc)--;
    (*argv)++;

    return arg;
}

void print_usage(char* program) {
    printf("USAGE: %s <subcommand> <option>\n", program);
    printf("SUBCOMMANDS:\n");
    printf("    block     <option>  --initialize world as block\n");
    printf("    glider    <option>  --initialize world as glider\n");
    printf("    blinker   <option>  --initialize world as blinker\n");
    printf("    pulsar    <option>  --initialize world as pulsar\n");
    printf("    gun       <option>  --initialize world as glider gun\n");
    printf("    random    <option>  --initialize world randomly\n");
    printf("OPTIONS:\n");
    printf("    epileptic           --random colors\n");
}

void show_world(void (*init_function)(bool world[ROWS][COLUMNS]), bool world[ROWS][COLUMNS], char* option) {
    init_function(world);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Game of Life");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            bool newWorld[ROWS][COLUMNS] = {false};
            for (int i = 0; i < ROWS; ++i){
                for (int j = 0; j < COLUMNS; ++j){
                    int liveNeighbors = count_live_neighbors(world, i, j);

                    if (world[i][j]){
                        if (liveNeighbors == 2 || liveNeighbors == 3){
                            newWorld[i][j] = true;
                        }
                        else if (liveNeighbors < 2 || liveNeighbors > 3){
                            newWorld[i][j] = false;
                        }
                    }
                    else {
                        if (liveNeighbors == 3){
                            newWorld[i][j] = true;
                        }
                    }

                    if (newWorld[i][j]){
                        int posX = WIDTH / 2 - (COLUMNS * RECT_DIM) / 2 + j * RECT_DIM;
                        int posY = HEIGHT / 2 - (ROWS * RECT_DIM) / 2 + i * RECT_DIM;
                        option ?  DrawRectangle(posX, posY, RECT_DIM, RECT_DIM, COLORS[rand()%COLORS_LEN]) :  DrawRectangle(posX, posY, RECT_DIM, RECT_DIM, RAYWHITE);
                    }
                }
            }

            for (int i = 0; i < ROWS; ++i){
                for (int j = 0; j < COLUMNS; ++j){
                    world[i][j] = newWorld[i][j];
                }
            }
            usleep(DELAY);
        }
        EndDrawing();
    }

    CloseWindow();
}

int main(int argc, char **argv) 
{
    char *program =  shift_args(&argc, &argv);
    
    char *command = shift_args(&argc, &argv);
    if (!command) {
        print_usage(program);
        return 0;
    }
    char *option = shift_args(&argc, &argv);
    if (option && !(strcmp(option, "epileptic") == 0))  {
        printf("INVALID OPTION: %s\n", option);
        print_usage(program);
        return 0;
    } 
    bool world[ROWS][COLUMNS] = {false};
    if (strcmp(command, "block") == 0) {
        show_world(init_world_as_block, world, option);
    } else if (strcmp(command, "glider") == 0 ) {
        show_world(init_world_as_glider, world, option);
    } else if (strcmp(command, "blinker") == 0 ) {
        show_world(init_world_as_blinker, world, option);
    } else if (strcmp(command, "pulsar") == 0 ) {
        show_world(init_world_as_pulsar, world, option);
    } else if (strcmp(command, "gun") == 0 ) {
        show_world(init_world_as_glider_gun, world, option);
    } else if (strcmp(command, "random") == 0 ) {
        show_world(init_world_randomly, world, option);
    } else {
        printf("INVALID COMMAND: %s\n", command);
        print_usage(program);
    }

    return 0;
}