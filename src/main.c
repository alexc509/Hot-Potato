#include "raylib.h"
#include <stdio.h>

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Hot Potato");

	Vector2 player1Pos = { 100, screenWidth / 2 };
	Vector2 player2Pos = { 700, screenWidth / 2 };
	float playerSpeed = 5;

	float lastCollisionTime = 0.0f;
	SetTargetFPS(60);


	while (!WindowShouldClose()) {
		
		if (IsKeyDown(KEY_D)) player1Pos.x += playerSpeed;
		if (IsKeyDown(KEY_A)) player1Pos.x -= playerSpeed;
		if (IsKeyDown(KEY_W)) player1Pos.y -= playerSpeed;
		if (IsKeyDown(KEY_S)) player1Pos.y += playerSpeed;

		if (IsKeyDown(KEY_RIGHT)) player2Pos.x += playerSpeed;
		if (IsKeyDown(KEY_LEFT)) player2Pos.x -= playerSpeed;
		if (IsKeyDown(KEY_UP)) player2Pos.y -= playerSpeed;
		if (IsKeyDown(KEY_DOWN)) player2Pos.y += playerSpeed;

		if (CheckCollisionRecs((Rectangle) { player1Pos.x, player1Pos.y, 20, 20 }, (Rectangle) { player2Pos.x, player2Pos.y, 20, 20 })) {
			if (GetTime() - lastCollisionTime >= .5) {
				printf("Collision Detected\n");
				lastCollisionTime = GetTime();
			}

		}
			BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawText("big chungus", 130, 200, 100, LIGHTGRAY);
		
			DrawRectangle(player1Pos.x, player1Pos.y, 20, 20, BLUE);
			DrawRectangle(player2Pos.x, player2Pos.y, 20, 20, RED);

		EndDrawing();
	}
	
	CloseWindow();
}