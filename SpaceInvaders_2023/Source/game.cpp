#include "game.h"
#include <vector>
#include <chrono>
constexpr float WallDistance = 250.0f;
constexpr float AlienDistance = 450.0f;
constexpr float ProjectileDistance = 130.0f;


Game::Game(State state) : background(600)
{
	Reset();
	player = Player();
	gameState = state;
}
void Game::End()
{
	//SAVE SCORE AND UPDATE SCOREBOARD
	Projectiles.clear();
	Walls.clear();
	Aliens.clear();
	newHighScore = CheckNewHighScore();
	gameState = State::ENDSCREEN;
}
void Game::Reset()
{
	Projectiles.clear();
	Walls.clear();
	Aliens.clear();
	player = Player();
	score = 0;
	const float wall_Y = static_cast<float>(GetScreenHeight()) - WallDistance;
	const float wall_Distance = static_cast<float>(GetScreenWidth()) / (wallCount + 1);

	for (int i = 0; i < wallCount; i++)
	{
		Wall newWall({ wall_Distance * (i + 1),wall_Y });
		Walls.push_back(newWall);
	}
	SpawnAliens();
}

void Game::Update()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		if (IsKeyReleased(KEY_SPACE))
		{
			gameState = State::GAMEPLAY;
		}
		break;
	case State::GAMEPLAY:
		if (IsKeyReleased(KEY_Q))
		{
			End();
		}

		player.Update();
		if (player.GetLives() < 1)
		{
			End();
		}
		for (Alien& alien : Aliens)
		{
			alien.Update();
			if (alien.GetYPosition() > GetScreenHeight() - player.GetPlayerBaseHeight())
			{
				End();
			}
		}
		if (Aliens.size() < 1)
		{
			SpawnAliens();
		}

		cornerPos = { 0, player.GetPlayerBaseHeight() };
		offset = lineLength(player.GetPosition(), cornerPos) * -1;
		background.Update(offset / 15);

		for (Projectile& projectile : Projectiles)
		{
			projectile.Update();
		}
		for (Wall& wall : Walls)
		{
			wall.Update();
		}

		//CHECK ALL COLLISONS HERE
		for (Projectile& projectile : Projectiles)
		{
			if (projectile.GetType() == EntityType::PLAYER_PROJECTILE)
			{
				for (Alien& alien : Aliens)
				{
					if (IsCollidingWith(projectile,alien))
					{
						projectile.SetActive(false);
						alien.SetActive(false);
						score += 100;
					}
				}
			}
			if (projectile.GetType() == EntityType::ENEMY_PROJECTILE)
			{
				if (IsCollidingWith(projectile,player))
				{
					projectile.SetActive(false);
					player.DecreaseLife();
				}
			}
			for (Wall& wall : Walls)
			{
				if (IsCollidingWith(projectile,wall))
				{
					projectile.SetActive(false);
					wall.TakeDamage();
				}
			}
		}


		if (IsKeyPressed(KEY_SPACE))
		{
			Projectile newProjectile({ player.GetXPosition(),static_cast<float>(GetScreenHeight()) - ProjectileDistance }, EntityType::PLAYER_PROJECTILE);
			Projectiles.push_back(newProjectile);
		}

		shootTimer += 1;
		if (shootTimer > 59)
		{
			int randomAlienIndex = 0;

			if (Aliens.size() > 1)
			{
				randomAlienIndex = rand() % Aliens.size();
			}

			Projectile newProjectile(Aliens[randomAlienIndex].GetPosition(), EntityType::ENEMY_PROJECTILE);
			newProjectile.OffsetEnemyProjectile();
			newProjectile.InverseSpeed();
			Projectiles.push_back(newProjectile);
			shootTimer = 0;
		}

		std::erase_if(Projectiles, [](Projectile& projectile) noexcept { return !projectile.GetActive(); });
		std::erase_if(Aliens, [](Alien& alien) noexcept { return !alien.GetActive(); });
		std::erase_if(Walls, [](Wall& wall) noexcept { return !wall.GetActive(); });

		break;
	case State::ENDSCREEN:
		//Code
		//Exit endscreen
		if (IsKeyReleased(KEY_ENTER) && !newHighScore)
		{
			Reset();
			gameState = State::STARTSCREEN;
		}



		if (newHighScore)
		{

			if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
			else mouseOnText = false;

			if (mouseOnText)
			{
				// Set the window's cursor to the I-Beam
				SetMouseCursor(MOUSE_CURSOR_IBEAM);

				// Get char pressed on the queue
				int key = GetCharPressed();

				// Check if more characters have been pressed on the same frame
				while (key > 0)
				{
					// NOTE: Only allow keys in range [32..125]
					if ((key >= 32) && (key <= 125) && (letterCount < 9))
					{
						name[letterCount] = (char)key;
						name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
						letterCount++;
					}

					key = GetCharPressed();  // Check next character in the queue
				}

				//Remove chars 
				if (IsKeyPressed(KEY_BACKSPACE))
				{
					letterCount--;
					if (letterCount < 0) letterCount = 0;
					name[letterCount] = '\0';
				}
			}
			else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

			if (mouseOnText)
			{
				framesCounter++;
			}
			else
			{
				framesCounter = 0;
			}

			// If the name is right legth and enter is pressed, exit screen by setting highscore to false and add 
			// name + score to scoreboard
			if (letterCount > 0 && letterCount < 9 && IsKeyReleased(KEY_ENTER))
			{
				std::string nameEntry(name);

				InsertNewHighScore(nameEntry);

				newHighScore = false;
			}


		}



		break;
	default:
		//SHOULD NOT HAPPEN
		break;
	}
}

bool const Game::IsCollidingWith(const Projectile& projectile, const Entity& entity)  {
	return CheckCollision(entity.GetPosition(), entity.GetRadius(), projectile.GetLineStart(), projectile.GetLineEnd());
}

void Game::Render()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		//Code
		DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
		DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
		break;
	case State::GAMEPLAY:
		//Code
		//background render LEAVE THIS AT TOP
		background.Render();

		DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
		DrawText(TextFormat("Lives: %i", player.GetLives()), 50, 70, 40, YELLOW);
 
		player.Render(resources.shipTextures[player.GetTextureFrame()]->get()); 
		for (Projectile projectile: Projectiles)
		{
			projectile.Render(resources.laserTexture->get());
		}
		for (Wall wall : Walls)
		{
			wall.Render(resources.barrierTexture->get());
		}
		for (Alien alien : Aliens)
		{
			alien.Render(resources.alienTexture->get());
		}

		break;
	case State::ENDSCREEN:
		if (newHighScore)
		{
			DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);
			DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);
			DrawRectangleRec(textBox, LIGHTGRAY);
			if (mouseOnText)
			{
				DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
			}
			else
			{
				DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
			}

			//Draw the name being typed out
			DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

			//Draw the text explaining how many characters are used
			DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 8), 600, 600, 20, YELLOW);

			if (mouseOnText)
			{
				if (letterCount < 9)
				{
					// Draw blinking underscore char
					if (((framesCounter / 20) % 2) == 0)
					{
						DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
					}

				}
				else
				{
					//Name needs to be shorter
					DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
				}
				
			}

			// Explain how to continue when name is input
			if (letterCount > 0 && letterCount < 9)
			{
				DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
			}

		}
		else {
			// If no highscore or name is entered, show scoreboard and call it a day
			DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);

			DrawText("LEADERBOARD", 50, 100, 40, YELLOW);

			for (int i = 0; i < Leaderboard.size(); i++)
			{
				char* tempNameDisplay = Leaderboard[i].name.data();
				DrawText(tempNameDisplay, 50, 140 + (i * 40), 40, YELLOW);
				DrawText(TextFormat("%i", Leaderboard[i].score), 350, 140 + (i * 40), 40, YELLOW);
			}
		}
		break;
	default:
		//SHOULD NOT HAPPEN
		break;
	}
}

void Game::SpawnAliens()
{
	for (int row = 0; row < formationHeight; row++) {
		for (int col = 0; col < formationWidth; col++) {
			Alien newAlien = Alien( { formationX + AlienDistance + (col * alienSpacing), static_cast<float>(formationY + (row * alienSpacing)) });
			Aliens.push_back(newAlien);
		}
	}

}

bool Game::CheckNewHighScore()
{
	if (score > Leaderboard[4].score)
	{
		return true;
	}

	return false;
}

void Game::InsertNewHighScore(std::string name)
{
	PlayerData newData;
	newData.name = name;
	newData.score = score;

	for (int i = 0; i < Leaderboard.size(); i++)
	{
		if (newData.score > Leaderboard[i].score)
		{

			Leaderboard.insert(Leaderboard.begin() + i, newData);

			Leaderboard.pop_back();

			i = Leaderboard.size();

		}
	}
}





