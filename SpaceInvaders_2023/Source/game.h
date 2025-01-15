#pragma once
#include "Resources.h"
#include "MathTools.h"
#include <string>
#include "Player.h"
#include "Walls.h"
#include "Alien.h"
#include "Background.h"
#include "Projectile.h"
#include "Collisions.h"
enum struct State
{
	STARTSCREEN,
	GAMEPLAY,
	ENDSCREEN
};
struct PlayerData
{
	std::string name;
	int score;
};

class Game
{
	State gameState = {};
	int score;
	int wallCount = 5;
	float shootTimer = 0;
	int formationWidth = 8;
	int formationHeight = 5;
	int alienSpacing = 80;
	int formationX = 100;
	int formationY = 50;
	bool newHighScore = false;
	char name[9 + 1] = "\0";  
	int letterCount = 0;
	int framesCounter = 0;
public:
	bool mouseOnText;
	Vector2 cornerPos;
	float offset;


	Resources resources;
	Rectangle textBox = { 600, 500, 225, 50 };
	Player player;

	std::vector<Projectile> Projectiles;
	std::vector<Wall> Walls;
	std::vector<Alien> Aliens;
	std::vector<PlayerData> Leaderboard = { {"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100} };
	Background background;

	Game(State state);
	void End();
	void Reset();
	void Update();
	void Render();
	void SpawnAliens();
	bool CheckNewHighScore();
	void InsertNewHighScore(std::string name);
	bool const IsCollidingWith(const Projectile& projectile, const Entity& entity);


};