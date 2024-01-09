// https://it.wikipedia.org/wiki/Gioco_della_vita
// TODO: aggungi logica per poter creare pattern, lavagna nera e cliccando con il mouse vengono aggiunti i pixel
// TODO: aggiungi logica per poter salvare i pattern creati, poterli caricare da file per poi avviare la visualizzazione

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <raylib.h>
#include "init.h"


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

void print_usage(char *program)
{
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

void create_world()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Game of Life");
    SetTargetFPS(60);
    ClearBackground(BACKGROUND);

    bool world[ROWS][COLUMNS] = {false};

    while (!WindowShouldClose()){
        BeginDrawing();
        {
            // draw
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                int mouseX = GetMouseX() % COLUMNS * RECT_DIM;
                int mouseY = GetMouseY() % COLUMNS * RECT_DIM;
                world[mouseY / RECT_DIM][mouseX / RECT_DIM] = true;
                DrawRectangle(mouseX, mouseY, RECT_DIM, RECT_DIM, RAYWHITE);
            }

            // erase
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
                int mouseX = GetMouseX() % COLUMNS * RECT_DIM;
                int mouseY = GetMouseY() % COLUMNS * RECT_DIM;
                world[mouseY / RECT_DIM][mouseX / RECT_DIM] = false;
                DrawRectangle(mouseX, mouseY, RECT_DIM, RECT_DIM, BACKGROUND);
            }
            // clean everything
            if (IsKeyPressed(KEY_C)){
                ClearBackground(BACKGROUND);
                memset(world, 0, sizeof(world));
            }

            // run simulation
            if (IsKeyDown(KEY_R)){
                bool temp_world[ROWS][COLUMNS] = {false};

                for (int i = 0; i < ROWS; ++i){
                    for (int j = 0; j < COLUMNS; ++j){
                        int liveNeighbors = count_live_neighbors(world, i, j);

                        if (world[i][j]){
                            if (liveNeighbors == 2 || liveNeighbors == 3){
                                temp_world[i][j] = true;
                            }
                            else if (liveNeighbors < 2 || liveNeighbors > 3){
                                temp_world[i][j] = false;
                            }
                        }
                        else{
                            if (liveNeighbors == 3){
                                temp_world[i][j] = true;
                            }
                        }
                        if (temp_world[i][j]){
                            DrawRectangle(j * RECT_DIM, i * RECT_DIM, RECT_DIM, RECT_DIM, RAYWHITE);
                        }
                        else{
                            DrawRectangle(j * RECT_DIM, i * RECT_DIM, RECT_DIM, RECT_DIM, BACKGROUND);
                        }
                    }
                }
                memcpy(world, temp_world, sizeof(temp_world[0][0]) * ROWS * COLUMNS);
                usleep(DELAY);
            }
        }
        EndDrawing();
    }

    CloseWindow();
}

void show_world(void (*init_function)(bool world[ROWS][COLUMNS]), bool world[ROWS][COLUMNS], char *option)
{
    init_function(world);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Game of Life");
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        BeginDrawing();
        {
            ClearBackground(BACKGROUND);

            bool temp_world[ROWS][COLUMNS] = {false};

            for (int i = 0; i < ROWS; ++i){
                for (int j = 0; j < COLUMNS; ++j){
                    int liveNeighbors = count_live_neighbors(world, i, j);

                    if (world[i][j]){
                        if (liveNeighbors == 2 || liveNeighbors == 3){
                            temp_world[i][j] = true;
                        }
                        else if (liveNeighbors < 2 || liveNeighbors > 3){
                            temp_world[i][j] = false;
                        }
                    }
                    else{
                        if (liveNeighbors == 3){
                            temp_world[i][j] = true;
                        }
                    }
                    if (temp_world[i][j] == true){
                        int posX = WIDTH / 2 - (COLUMNS * RECT_DIM) / 2 + j * RECT_DIM;
                        int posY = HEIGHT / 2 - (ROWS * RECT_DIM) / 2 + i * RECT_DIM;
                        option ? DrawRectangle(posX, posY, RECT_DIM, RECT_DIM, COLORS[rand() % COLORS_LEN]) : DrawRectangle(posX, posY, RECT_DIM, RECT_DIM, RAYWHITE);
                    }
                }
            }

            memcpy(world, temp_world, sizeof(temp_world[0][0]) * ROWS * COLUMNS);
            usleep(DELAY);
        }
        EndDrawing();
    }

    CloseWindow();
}

int main(int argc, char **argv)
{
    char *program = *argv++;

    char *command = *argv++;
    if (command == NULL){
        print_usage(program);
        return 0;
    }

    char *option = *argv++;
    if (option != NULL && !(strcmp(option, "epileptic") == 0)){
        printf("INVALID OPTION: %s\n", option);
        print_usage(program);
        return 0;
    }

    bool world[ROWS][COLUMNS] = {false};
    if (strcmp(command, "block") == 0){
        show_world(init_world_as_block, world, option);
    }
    else if (strcmp(command, "glider") == 0){
        show_world(init_world_as_glider, world, option);
    }
    else if (strcmp(command, "blinker") == 0){
        show_world(init_world_as_blinker, world, option);
    }
    else if (strcmp(command, "pulsar") == 0){
        show_world(init_world_as_pulsar, world, option);
    }
    else if (strcmp(command, "gun") == 0){
        show_world(init_world_as_glider_gun, world, option);
    }
    else if (strcmp(command, "random") == 0){
        show_world(init_world_randomly, world, option);
    }
    else if (strcmp(command, "create") == 0){
        create_world();
    }
    else{
        printf("INVALID COMMAND: %s\n", command);
        print_usage(program);
    }

    return 0;
}