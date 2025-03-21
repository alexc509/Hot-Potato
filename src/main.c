#include "raylib.h"
#include <stdio.h>

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Hot Potato");

	typedef enum GameScreen { GAMEPLAY, ENDING } GameScreen;

	Texture2D potatoSprite = LoadTexture("resources/potato.png");
	Texture2D ExplosionEndScreen = LoadTexture("resources/explosion.png");

	InitAudioDevice();
	bool soundPlayed = false;

	Sound explosionSound  = LoadSound("resources/explosion.mp3");

	Vector2 player1Pos = { 100, screenWidth / 2 };
	Vector2 player2Pos = { 700, screenWidth / 2 };
	float player1Speed = 5;
	float player2Speed = 5;
	float playerSize = 20;
	bool player1HasPotato = true;
	bool player2HasPotato = false;
	float PotatoExplosionTime = 15.0f;

	float lastCollisionTime = 0.0f;
	SetTargetFPS(60);

	GameScreen currentScreen = GAMEPLAY;

	while (!WindowShouldClose()) {

		switch (currentScreen) {
			case GAMEPLAY:
			{
				float PotatoTimer = (PotatoExplosionTime - GetTime()) + lastCollisionTime;
				char sPotatoTimer[50];
				gcvt(PotatoTimer, 3, sPotatoTimer);

				if (PotatoTimer <= 0) {
					currentScreen = ENDING;
				}

				if (player1HasPotato) {
					player1Speed = 6;
					player2Speed = 5;
				}
				else {
					player2Speed = 6;
					player1Speed = 5;
				}

				if (IsKeyDown(KEY_D)) player1Pos.x += player1Speed;
				if (IsKeyDown(KEY_A)) player1Pos.x -= player1Speed;
				if (IsKeyDown(KEY_W)) player1Pos.y -= player1Speed;
				if (IsKeyDown(KEY_S)) player1Pos.y += player1Speed;

				if (player1Pos.x < 0) player1Pos.x = 0;
				if (player1Pos.x > screenWidth - playerSize) player1Pos.x = screenWidth - playerSize;
				if (player1Pos.y < 0) player1Pos.y = 0;
				if (player1Pos.y > screenHeight - playerSize) player1Pos.y = screenHeight - playerSize;

				if (IsKeyDown(KEY_RIGHT)) player2Pos.x += player2Speed;
				if (IsKeyDown(KEY_LEFT)) player2Pos.x -= player2Speed;
				if (IsKeyDown(KEY_UP)) player2Pos.y -= player2Speed;
				if (IsKeyDown(KEY_DOWN)) player2Pos.y += player2Speed;

				if (player2Pos.x < 0) player2Pos.x = 0;
				if (player2Pos.x > screenWidth - playerSize) player2Pos.x = screenWidth - playerSize;
				if (player2Pos.y < 0) player2Pos.y = 0;
				if (player2Pos.y > screenHeight - playerSize) player2Pos.y = screenHeight - playerSize;


				if (CheckCollisionRecs((Rectangle) { player1Pos.x, player1Pos.y, 20, 20 }, (Rectangle) { player2Pos.x, player2Pos.y, 20, 20 })) {
					if (GetTime() - lastCollisionTime >= .5) {
						PotatoTimer = 15.0f;
						lastCollisionTime = GetTime();
						player1HasPotato = !player1HasPotato;
						player2HasPotato = !player2HasPotato;
					}
				}
				BeginDrawing();

				ClearBackground(RAYWHITE);

				DrawText("big chungus", 130, 200, 100, LIGHTGRAY);

				DrawRectangle(player1Pos.x, player1Pos.y, playerSize, playerSize, BLUE);
				DrawRectangle(player2Pos.x, player2Pos.y, playerSize, playerSize, RED);

				DrawText((sPotatoTimer), 350, 30, 30, BLACK);

				if (player1HasPotato) {
					DrawTexture(potatoSprite, player1Pos.x - 5, player1Pos.y - 30, WHITE);
				}
				else {
					DrawTexture(potatoSprite, player2Pos.x - 5, player2Pos.y - 30, WHITE);
				}

				EndDrawing();
			}	break;
			case ENDING: 
			{
				if (!soundPlayed) PlaySound(explosionSound);
				soundPlayed = true;
				BeginDrawing();
					ClearBackground(RAYWHITE);
					DrawTexture(ExplosionEndScreen, 0, 0, WHITE);
					if (player1HasPotato) {
						DrawText("Player 2 Wins", 275, 10, 40, RED);
					}
					else {
						DrawText("Player 1 Wins", 275, 10, 40, BLUE);
					}
				EndDrawing();
			} break;

		}

	}
	UnloadSound(explosionSound);
	CloseAudioDevice();

	CloseWindow();

	return 0;
}