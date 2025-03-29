#include "raylib.h"
#include <stdio.h>
#include <time.h>

int main(void) {

	srand(time(NULL));

	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Hot Potato");

	typedef enum GameScreen { GAMEPLAY, ENDING } GameScreen;

	Texture2D player1Sprite = LoadTexture("resources/player_1.png");
	Texture2D player2Sprite = LoadTexture("resources/player_2.png");
	Texture2D potatoSprite = LoadTexture("resources/potato.png");
	Texture2D ExplosionEndScreen = LoadTexture("resources/explosion.png");
	Texture2D portalSprite = LoadTexture("resources/portal.png");

	InitAudioDevice();

	bool soundPlayed = false;

	Sound explosionSound  = LoadSound("resources/explosion.mp3");

	Vector2 player1Pos = { 100, screenWidth / 2 };
	Vector2 player2Pos = { 700, screenWidth / 2 };
	float player1Speed = 5;
	float player2Speed = 5;
	float playerSize = 32;


	bool player1HasPotato = true;
	bool player2HasPotato = false;
	float PotatoExplosionTime = 15.0f;
	float portalRespawnTime = 10.0f;
	float dashDelayTime = 8.0f;

	
	float player1lastDashTime = 0.0f;
	float player2lastDashTime = 0.0f;
	float player1dashCooldown = 8.0f;
	float player2dashCooldown = 8.0f;

	float lastPortalSpawnTime = 0.0f;
	Rectangle portal1 = { 0, 0, 20, 30 };
	portal1.x = rand() % screenWidth;
	portal1.y = rand() % screenHeight;
	Rectangle portal2 = { 0, 0, 20, 30 };
	portal2.x = rand() % screenWidth;
	portal2.y = rand() % screenHeight;

	char lastPressedMoveKeyPlayer1 = '\0';
	char lastPressedMoveKeyPlayer2[50];

	float lastCollisionTime = 0.0f;
	SetTargetFPS(60);

	GameScreen currentScreen = GAMEPLAY;

	while (!WindowShouldClose()) {

		switch (currentScreen) {

			case GAMEPLAY:
			{
				printf("Player 1 Dash Cooldown: %f\nPlayer 2 Dash Cooldown: %f\n\n", player1dashCooldown, player2dashCooldown);
				float PotatoTimer = (PotatoExplosionTime - GetTime()) + lastCollisionTime;
				float portalTimer = (portalRespawnTime - GetTime()) + lastPortalSpawnTime;
				player1dashCooldown = dashDelayTime - (GetTime() - player1lastDashTime);
				player2dashCooldown = dashDelayTime - (GetTime() - player2lastDashTime);

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

				if (IsKeyDown(KEY_D)) {
					player1Pos.x += player1Speed;
					lastPressedMoveKeyPlayer1 = 'D';
				}
				if (IsKeyDown(KEY_A)) {
					player1Pos.x -= player1Speed;
					lastPressedMoveKeyPlayer1 = 'A';
				}
				if (IsKeyDown(KEY_W)) {
					player1Pos.y -= player1Speed;
					lastPressedMoveKeyPlayer1 = 'W'; 
				}
				if (IsKeyDown(KEY_S)) {
					player1Pos.y += player1Speed;
					lastPressedMoveKeyPlayer1 = 'S';
				}

				if (IsKeyPressed(KEY_SPACE) && player1dashCooldown <= 0) {
					player1lastDashTime = GetTime();
					switch (lastPressedMoveKeyPlayer1) {
						case 'D':
							player1Pos.x += 150;
							break;
						case 'A':
							player1Pos.x -= 150;
							break;
						case 'W':
							player1Pos.y -= 150;
							break;
						case 'S':
							player1Pos.y += 150;
							break;
						}
				}

				if (player1Pos.x < 0) player1Pos.x = 0;
				if (player1Pos.x > screenWidth - playerSize) player1Pos.x = screenWidth - playerSize;
				if (player1Pos.y < 0) player1Pos.y = 0;
				if (player1Pos.y > screenHeight - playerSize) player1Pos.y = screenHeight - playerSize;



				if (IsKeyDown(KEY_RIGHT)) {
					player2Pos.x += player2Speed;
					strcpy(lastPressedMoveKeyPlayer2, "KEY_RIGHT");
				}
				if (IsKeyDown(KEY_LEFT)) {
					player2Pos.x -= player2Speed;
					strcpy(lastPressedMoveKeyPlayer2, "KEY_LEFT");
				}
				if (IsKeyDown(KEY_UP)) {
					player2Pos.y -= player2Speed;
					strcpy(lastPressedMoveKeyPlayer2, "KEY_UP");
				}
				if (IsKeyDown(KEY_DOWN)) {
					player2Pos.y += player2Speed;
					strcpy(lastPressedMoveKeyPlayer2, "KEY_DOWN");
				}

				if (IsKeyPressed(KEY_RIGHT_CONTROL) && player2dashCooldown <= 0) {
					player2lastDashTime = GetTime();
					if (strcmp(lastPressedMoveKeyPlayer2, "KEY_RIGHT") == 0) {
						player2Pos.x += 150;
					}
					else if (strcmp(lastPressedMoveKeyPlayer2, "KEY_LEFT") == 0) {
						player2Pos.x -= 150;
					}
					else if (strcmp(lastPressedMoveKeyPlayer2, "KEY_UP") == 0) {
						player2Pos.y -= 150;
					}
					else if (strcmp(lastPressedMoveKeyPlayer2, "KEY_DOWN") == 0) {
						player2Pos.y += 150;
					}
				}

				if (player2Pos.x < 0) player2Pos.x = 0;
				if (player2Pos.x > screenWidth - playerSize) player2Pos.x = screenWidth - playerSize;
				if (player2Pos.y < 0) player2Pos.y = 0;
				if (player2Pos.y > screenHeight - playerSize) player2Pos.y = screenHeight - playerSize;


				if (CheckCollisionRecs((Rectangle) { player1Pos.x, player1Pos.y, 32, 32 }, (Rectangle) { player2Pos.x, player2Pos.y, 32, 32 })) {
					if (GetTime() - lastCollisionTime >= .5) {
						PotatoTimer = 15.0f;
						lastCollisionTime = GetTime();
						player1HasPotato = !player1HasPotato;
						player2HasPotato = !player2HasPotato;
						PotatoExplosionTime -= .25;
					}
				}

				if (CheckCollisionRecs((Rectangle) { player1Pos.x, player1Pos.y, 20, 20 }, (Rectangle) { portal1.x, portal1.y, 20, 30 })) {
					player1Pos.x = portal2.x;
					player1Pos.y = portal2.y;
					portal1.x = -999;
					portal1.y = -999;
					portal2.x = -999;
					portal2.y = -999;
				}
				if (CheckCollisionRecs((Rectangle) { player1Pos.x, player1Pos.y, 20, 20 }, (Rectangle) { portal2.x, portal2.y, 20, 30 })) {
					player1Pos.x = portal1.x;
					player1Pos.y = portal1.y;
					portal1.x = -999;
					portal1.y = -999;
					portal2.x = -999;
					portal2.y = -999;
				}

				if (CheckCollisionRecs((Rectangle) { player2Pos.x, player2Pos.y, 20, 20 }, (Rectangle) { portal1.x, portal1.y, 20, 30 })) {
					player2Pos.x = portal2.x;
					player2Pos.y = portal2.y;
					portal1.x = -999;
					portal1.y = -999;
					portal2.x = -999;
					portal2.y = -999;
				}
				if (CheckCollisionRecs((Rectangle) { player2Pos.x, player2Pos.y, 20, 20 }, (Rectangle) { portal2.x, portal2.y, 20, 30 })) {
					player2Pos.x = portal1.x;
					player2Pos.y = portal1.y;
					portal1.x = -999;
					portal1.y = -999;
					portal2.x = -999;
					portal2.y = -999;
				}

				if (portalTimer <= 0) {
					portal1.x = rand() % screenWidth;
					portal1.y = rand() % screenHeight;
					portal2.x = rand() % screenWidth;
					portal2.y = rand() % screenHeight;
					portalTimer = portalRespawnTime;
					lastPortalSpawnTime = GetTime();
				}

				BeginDrawing();

				ClearBackground(RAYWHITE);

					DrawText("big chungus", 130, 200, 100, LIGHTGRAY);

					DrawRectangle(player1Pos.x, player1Pos.y, playerSize, playerSize, BLUE);
					DrawRectangle(player2Pos.x, player2Pos.y, playerSize, playerSize, RED);
					DrawTexture(player1Sprite, player1Pos.x, player1Pos.y, WHITE);
					DrawTexture(player2Sprite, player2Pos.x, player2Pos.y, WHITE);

					DrawText((sPotatoTimer), 350, 30, 30, BLACK);

					if (player1HasPotato) {
						DrawTexture(potatoSprite, player1Pos.x - 5, player1Pos.y - 30, WHITE);
					}
					else {
						DrawTexture(potatoSprite, player2Pos.x - 5, player2Pos.y - 30, WHITE);
					}

					DrawTexture(portalSprite, portal1.x, portal1.y, WHITE);
					DrawTexture(portalSprite, portal2.x, portal2.y, WHITE);

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