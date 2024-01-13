#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 2400
#define SCREEN_HEIGHT 1200
#define RECT_DIM 2
#define IMG_CELL_SIZE (2 + 1)
#define PADDING 20
#define ROWS 5000
#define COLS 5150
static bool board[ROWS][COLS] = {0};
static bool temp_board[ROWS][COLS] = {0};

void init_board(Image image)
{
	printf("image: (%d %d) Matrix: (%d %d)\n", image.width, image.height, COLS, ROWS);
	assert(image.height <= (int) ROWS && image.width <= (int)COLS && "ERROR: Image resolution is greater than board size");
	unsigned char *pixel_data = image.data;
	for (int i = 0; i < image.height; i++)
	{
		for (int j = 0; j < image.width; j++)
		{
			size_t offset = (i * image.width + j) * 4;
			int r = pixel_data[offset];
			int g = pixel_data[offset + 1];
			int b = pixel_data[offset + 2];

			if (g > r && g > b)
			{
				board[i + PADDING][j + PADDING] = true;
			}
			else
			{
				board[i + PADDING][j + PADDING] = false;
			}
		}
	}
}

int count_live_neighbors(size_t row, size_t col)
{
	int count = 0;
	for (size_t i = row -IMG_CELL_SIZE; i <= row + IMG_CELL_SIZE; i+=IMG_CELL_SIZE)
	{
		for (size_t j = col - IMG_CELL_SIZE; j <= col + IMG_CELL_SIZE; j+=IMG_CELL_SIZE)
		{
			if ( i < ROWS && j < COLS && !(i == row && j == col) && board[i][j])
			{
				count++;
			}
		}
	}
	return count;
}

int main(int argc, char **argv)
{
	(void)argc;
	const char *program = *argv++;
	assert(program != NULL);

	const char *filename = *argv++;
	if (filename == NULL)
	{
		printf("ERROR: You need to provide an image");
		exit(EXIT_FAILURE);
	}

	// set up the window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's Game of Life");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);

	Image image = LoadImage(filename);

	Camera2D cam = {0};
	cam.zoom = 1;
	float boardCenterX = image.width * RECT_DIM * 0.5f - SCREEN_WIDTH / 2;
	float boardCenterY = image.height * RECT_DIM * 0.5f - SCREEN_HEIGHT / 2;
	cam.target.x = boardCenterX;
	cam.target.y = boardCenterY;
	init_board(image);

	// game loop
	while (!WindowShouldClose())
	{
		// translate based on left click
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f / cam.zoom);
			cam.target = Vector2Add(cam.target, delta);
		}

		// zoom based on wheel
		float wheel = GetMouseWheelMove();
		if (wheel != 0)
		{
			Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cam);
			cam.offset = GetMousePosition();
			cam.target = mouseWorldPos;

			// zoom
			cam.zoom += wheel * 0.125f;
			if (cam.zoom < 0.125f)
				cam.zoom = 0.125f;
		}

		// drawing
		ClearBackground(BLACK);
		BeginDrawing();
		BeginMode2D(cam);

		memset(temp_board, 0, sizeof(temp_board));

		for (size_t i = 0 + PADDING; i < ROWS; i+= IMG_CELL_SIZE)
		{
			for (size_t j = 0 + PADDING; j < COLS; j+=IMG_CELL_SIZE)
			{
				int liveNeighbors = count_live_neighbors(i, j);
				// DEBUG: to see the initial state of the pattern
				// if (board[i][j])
				// 	temp_board[i][j] = true;
				// else
				// 	temp_board[i][j] = false;
				if (board[i][j])
				{

					if (liveNeighbors == 2 || liveNeighbors == 3)
					{
						temp_board[i][j] = true;
					}
					else
					{
						temp_board[i][j] = false;
					}
				}
				else
				{
					if (liveNeighbors == 3)
					{
						temp_board[i][j] = true;
					}
				}
				if (temp_board[i][j] == true)
				{
					DrawRectangle(j * RECT_DIM, i * RECT_DIM, RECT_DIM * IMG_CELL_SIZE - 1, RECT_DIM * IMG_CELL_SIZE - 1, RAYWHITE);
				}
			}
		}

		memcpy(board, temp_board, sizeof(temp_board));
		WaitTime(0.05);
		EndMode2D();
		EndDrawing();
	}

	// cleanup
	CloseWindow();
	return 0;
}
