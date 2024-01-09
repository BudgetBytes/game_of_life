// https://it.wikipedia.org/wiki/Gioco_della_vita
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
    for (size_t i = row - 1; i <= row + 1; ++i)
    {
        for (size_t j = col - 1; j <= col + 1; ++j)
        {
            if (i >= 0 && i < ROWS && j >= 0 && j < COLUMNS && !(i == row && j == col) && world[i][j])
            {
                count++;
            }
        }
    }
    return count;
}

void print_usage(char *program)
{
    printf("USAGE: %s <subcommand>\n", program);
    printf("SUBCOMMANDS:\n");
    printf("    block               --initialize world as block\n");
    printf("    glider              --initialize world as glider\n");
    printf("    blinker             --initialize world as blinker\n");
    printf("    pulsar              --initialize world as pulsar\n");
    printf("    gun                 --initialize world as glider gun\n");
    printf("    random              --initialize world randomly\n");
    printf("    create              --initialize empty world to draw and simulate\n");
}

void create_world()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Game of Life");
    SetTargetFPS(60);
    ClearBackground(BACKGROUND);

    bool world[ROWS][COLUMNS] = {false};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            if ( IsKeyDown(KEY_D) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                for (size_t i = 0, j = 0; i < COLUMNS && j < ROWS; ++i, ++j)
                {
                    int mouseY = i * RECT_DIM;
                    int mouseX = j * RECT_DIM;
                    world[i][j] = true;
                    DrawRectangle(mouseX, mouseY, RECT_DIM, RECT_DIM, RAYWHITE);
                }
            }

            if (IsKeyDown(KEY_D) && IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
            {
                for (size_t i = COLUMNS - 1, j = 0; i >= 0 && j < ROWS; --i, ++j)
                {
                    int mouseY = i * RECT_DIM;
                    int mouseX = j * RECT_DIM;
                    world[i][j] = true;
                    DrawRectangle(mouseX, mouseY, RECT_DIM, RECT_DIM, RAYWHITE);
                }
            }

            // Draw horizontal line
            if (IsKeyDown(KEY_H) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                int mouseY = GetMouseY() % ROWS * RECT_DIM;

                for (size_t i = 0; i < COLUMNS; ++i)
                {
                    int mouseX = i * RECT_DIM;
                    world[mouseY / RECT_DIM][i] = true;
                    DrawRectangle(mouseX, mouseY, RECT_DIM, RECT_DIM, RAYWHITE);
                }
            }

            // Draw vertical line
            if (IsKeyDown(KEY_V) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                int mouseX = GetMouseX() % COLUMNS * RECT_DIM;

                for (size_t i = 0; i < ROWS; ++i)
                {
                    int mouseY = i * RECT_DIM;
                    world[i][mouseX / RECT_DIM] = true;
                    DrawRectangle(mouseX, mouseY, RECT_DIM, RECT_DIM, RAYWHITE);
                }
            }

            // draw freely
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                int mouseX = GetMouseX() % COLUMNS * RECT_DIM;
                int mouseY = GetMouseY() % ROWS * RECT_DIM;
                world[mouseY / RECT_DIM][mouseX / RECT_DIM] = true;
                DrawRectangle(mouseX, mouseY, RECT_DIM, RECT_DIM, RAYWHITE);
            }

            // erase
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            {
                int mouseX = GetMouseX() % COLUMNS * RECT_DIM;
                int mouseY = GetMouseY() % ROWS * RECT_DIM;
                world[mouseY / RECT_DIM][mouseX / RECT_DIM] = false;
                DrawRectangle(mouseX, mouseY, RECT_DIM, RECT_DIM, BACKGROUND);
            }

            // delete everything
            if (IsKeyReleased(KEY_C))
            {
                ClearBackground(BACKGROUND);
                memset(world, 0, sizeof(world));
            }

            // next generation
            if (IsKeyDown(KEY_N))
            {

                bool temp_world[ROWS][COLUMNS] = {false};

                for (size_t i = 0; i < ROWS; ++i)
                {
                    for (size_t j = 0; j < COLUMNS; ++j)
                    {
                        int liveNeighbors = count_live_neighbors(world, i, j);

                        if (world[i][j])
                        {
                            if (liveNeighbors == 2 || liveNeighbors == 3)
                            {
                                temp_world[i][j] = true;
                            }
                            else
                            {
                                temp_world[i][j] = false;
                            }
                        }
                        else
                        {
                            if (liveNeighbors == 3)
                            {
                                temp_world[i][j] = true;
                            }
                        }
                        if (temp_world[i][j])
                        {
                            DrawRectangle(j * RECT_DIM, i * RECT_DIM, RECT_DIM, RECT_DIM, RAYWHITE);
                        }
                        else
                        {
                            DrawRectangle(j * RECT_DIM, i * RECT_DIM, RECT_DIM, RECT_DIM, BACKGROUND);
                        }
                    }
                }
                memcpy(world, temp_world, sizeof(temp_world));
                usleep(DELAY);
            }
        }
        EndDrawing();
    }

    CloseWindow();
}

void show_world(void (*init_function)(bool world[ROWS][COLUMNS]), bool world[ROWS][COLUMNS])
{
    init_function(world);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Game of Life");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BACKGROUND);

            bool temp_world[ROWS][COLUMNS] = {false};

            for (size_t i = 0; i < ROWS; ++i)
            {
                for (size_t j = 0; j < COLUMNS; ++j)
                {
                    int liveNeighbors = count_live_neighbors(world, i, j);

                    if (world[i][j])
                    {
                        if (liveNeighbors == 2 || liveNeighbors == 3)
                        {
                            temp_world[i][j] = true;
                        }
                        else
                        {
                            temp_world[i][j] = false;
                        }
                    }
                    else
                    {
                        if (liveNeighbors == 3)
                        {
                            temp_world[i][j] = true;
                        }
                    }
                    if (temp_world[i][j] == true)
                    {
                        DrawRectangle(j * RECT_DIM, i * RECT_DIM, RECT_DIM, RECT_DIM, RAYWHITE);
                    }
                }
            }

            memcpy(world, temp_world, sizeof(temp_world));
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
    if (command == NULL)
    {
        print_usage(program);
        return 0;
    }

    bool world[ROWS][COLUMNS] = {false};
    if (strcmp(command, "block") == 0)
    {
        show_world(init_world_as_block, world);
    }
    else if (strcmp(command, "glider") == 0)
    {
        show_world(init_world_as_glider, world);
    }
    else if (strcmp(command, "blinker") == 0)
    {
        show_world(init_world_as_blinker, world);
    }
    else if (strcmp(command, "pulsar") == 0)
    {
        show_world(init_world_as_pulsar, world);
    }
    else if (strcmp(command, "gun") == 0)
    {
        show_world(init_world_as_glider_gun, world);
    }
    else if (strcmp(command, "random") == 0)
    {
        show_world(init_world_randomly, world);
    }
    else if (strcmp(command, "create") == 0)
    {
        create_world();
    }
    else
    {
        printf("INVALID COMMAND: %s\n", command);
        print_usage(program);
    }

    return 0;
}