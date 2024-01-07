#include <stdio.h>
#include "raylib.h"

int main(void) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Test Raylib");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawText("Test Raylib", screenWidth/2, screenHeight/2, 69, LIGHTGRAY);
        }
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}