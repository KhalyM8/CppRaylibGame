#include "raylib.h"
#include <cstdlib>
#include <raymath.h>
#include <iostream>

struct Player {
	float x, y;
	float speed;
	float radius;

	void Draw() {
		DrawCircle((int)x, (int)y, radius, WHITE);
	}
};

struct Apple {
	int x, y;

	void Draw() {
		DrawCircle(x,y, 5, GREEN);
	}
};

struct Enemy {
	Vector2 enemyPos = { 300,300 };
	float speed;

	void Draw() {
		DrawCircle((int)enemyPos.x, (int)enemyPos.y, 10, RED);
	}
};
struct PathfindingPath {
	float startx, starty, endx, endy;

	void Draw() {
		DrawLine(startx, starty, endx,endy,PURPLE);
	}
};

int main() {
	InitWindow(800, 600, "PacMan");
	SetWindowState(FLAG_VSYNC_HINT);
	int screenHeight = GetScreenHeight();
	int screenWidth = GetScreenWidth();

	Player player;
	player.x = screenWidth / 2;
	player.y = screenHeight / 2;
	player.speed = 125;
	player.radius = 10;

	Apple apple;
	apple.x = rand() % screenWidth;
	apple.y = rand()% screenHeight;

	Enemy enemy;
	enemy.enemyPos.x = player.x - 100;
	enemy.enemyPos.y = player.y - 100;
	enemy.speed = 0.1;

	const char* winnerText = nullptr;
	int points = 0;


	while (!WindowShouldClose()) {
		float delta = GetFrameTime();
		enemy.speed += 0.001 * delta;
		if (!winnerText) {
			if (enemy.enemyPos.x > player.x) {
				enemy.enemyPos.x += player.x * enemy.speed * delta * -2;
			}
			else {
				enemy.enemyPos.x += player.x * enemy.speed * delta;
			}

			if (enemy.enemyPos.y > player.y) {
				enemy.enemyPos.y += player.y * enemy.speed * delta * -2;
			}
			else {
				enemy.enemyPos.y += player.y * enemy.speed * delta;
			}
		}


		if (IsKeyDown(KEY_UP)) {
			if (!winnerText) {
				player.y -= player.speed * delta;
			}
		}
		if (IsKeyDown(KEY_DOWN)) {
			if (!winnerText) {
				player.y += player.speed * delta;
			}
		}
		if (IsKeyDown(KEY_LEFT)) {
			if (!winnerText) {
				player.x -= player.speed * delta;
			}
		}
		if (IsKeyDown(KEY_RIGHT)) {
			if (!winnerText) {
				player.x += player.speed * delta;
			}
		}

		if (CheckCollisionCircles(Vector2{ player.x, player.y }, player.radius, Vector2{ (float)apple.x,(float)apple.y }, 5)) {
				apple.x = rand() % screenWidth;
				apple.y = rand()% screenHeight;
				player.speed += 2;
				points += 1;
		}
		if (CheckCollisionCircles(Vector2{ player.x, player.y }, player.radius, Vector2{ (float)enemy.enemyPos.x,(float)enemy.enemyPos.y }, 10)) {
			winnerText = "You Lost";
		}

		if (winnerText && IsKeyPressed(KEY_SPACE)) {
			player.x = screenWidth / 2;
			player.y = screenHeight / 2;
			enemy.enemyPos.x = player.x - 100;
			enemy.enemyPos.y = player.y - 100;
			enemy.speed = 0.1;
			points = 0;
			winnerText = nullptr;
		}
		if (player.x > screenWidth) {
			player.x = 0;
		}
		if (player.x < 0) {
			player.x = screenWidth;
		}
		if (player.y < 0) {
			player.y = screenHeight;
		}
		if (player.y > screenHeight) {
			player.y = 0;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		player.Draw();
		apple.Draw();
		enemy.Draw();
		if (winnerText) {
			int textWidth = MeasureText(winnerText, 60);
			DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
		}
		DrawText(TextFormat("%i", points), 50, 50, 40, GREEN);



		DrawFPS(0, 0);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}