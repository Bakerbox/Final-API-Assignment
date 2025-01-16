#include "game.h"
#include <vector>

constexpr float WallDistance = 250.0f;
constexpr float AlienDistance = 450.0f;
constexpr float ProjectileDistance = 130.0f;
constexpr float SHOOT_INTERVAL = 60.0f;
constexpr int MaxNameLength = 8;
constexpr int LeaderboardRows = 5;

Game::Game(State state) : background(600)
{
    Reset();
    player = Player();
    gameState = state;
}

void Game::End() noexcept
{
    Projectiles.clear();
    Walls.clear();
    Aliens.clear();
    newHighScore = CheckNewHighScore();
    gameState = State::ENDSCREEN;
}

void Game::Reset() noexcept
{
    Projectiles.clear();
    Walls.clear();
    Aliens.clear();
    player = Player();
    score = 0;

    const float wall_Y = static_cast<float>(GetScreenHeight()) - WallDistance;
    const float wall_Spacing = static_cast<float>(GetScreenWidth()) / (wallCount + 1);

    for (int i = 0; i < wallCount; ++i)
    {
        Wall newWall({ wall_Spacing * (i + 1), wall_Y });
        Walls.push_back(newWall);
    }
    SpawnAliens();
}

void Game::Update() noexcept
{
    switch (gameState)
    {
    case State::STARTSCREEN:
        ShowStartScreen();
        break;

    case State::GAMEPLAY:
        HandleGameplay();
        break;

    case State::ENDSCREEN:
        ShowEndScreen();
        break;

    default:
        break;
    }
}

void Game::ShowStartScreen() noexcept
{
    if (IsKeyReleased(KEY_SPACE))
    {
        gameState = State::GAMEPLAY;
    }
}

void Game::HandleGameplay() noexcept
{
    HandlePlayerInput();
    UpdateEntities();
    HandleCollisions();
    SpawnNewAliensIfNecessary();
    UpdateBackground();
    HandleShooting();
}

void Game::HandlePlayerInput() noexcept
{
    if (IsKeyReleased(KEY_Q))
    {
        End();
    }
}

void Game::UpdateEntities() noexcept
{
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

    for (Projectile& projectile : Projectiles)
    {
        projectile.Update();
    }

    for (Wall& wall : Walls)
    {
        wall.Update();
    }
}

void Game::HandleCollisions() noexcept
{
    for (Projectile& projectile : Projectiles)
    {
        if (projectile.GetType() == EntityType::PLAYER_PROJECTILE)
        {
            HandlePlayerProjectileCollisions(projectile);
        }
        else if (projectile.GetType() == EntityType::ENEMY_PROJECTILE)
        {
            HandleEnemyProjectileCollisions(projectile);
        }
        HandleProjectileWallCollisions(projectile);
    }
}

void Game::HandlePlayerProjectileCollisions(Projectile& projectile) noexcept
{
    for (Alien& alien : Aliens)
    {
        if (IsCollidingWith(projectile, alien))
        {
            projectile.SetActive(false);
            alien.SetActive(false);
            score += 100;
        }
    }
}

void Game::HandleEnemyProjectileCollisions(Projectile& projectile) noexcept
{
    if (IsCollidingWith(projectile, player))
    {
        projectile.SetActive(false);
        player.DecreaseLife();
    }
}

void Game::HandleProjectileWallCollisions(Projectile& projectile) noexcept
{
    for (Wall& wall : Walls)
    {
        if (IsCollidingWith(projectile, wall))
        {
            projectile.SetActive(false);
            wall.TakeDamage();
        }
    }
}

void Game::SpawnNewAliensIfNecessary() noexcept
{
    if (Aliens.empty())
    {
        SpawnAliens();
    }
}

void Game::UpdateBackground() noexcept
{
    cornerPos = { 0, player.GetPlayerBaseHeight() };
    offset = lineLength(player.GetPosition(), cornerPos) * -1;
    background.Update(offset / 15);
}

void Game::HandleShooting() noexcept
{
    shootTimer += 1;
    if (shootTimer > SHOOT_INTERVAL - 1)
    {
        SpawnEnemyProjectile();
        shootTimer = 0;
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        SpawnPlayerProjectile();
    }

    CleanupInactiveEntities();
}

void Game::SpawnPlayerProjectile() noexcept
{
    Projectile newProjectile({ player.GetXPosition(), static_cast<float>(GetScreenHeight()) - ProjectileDistance }, EntityType::PLAYER_PROJECTILE);
    Projectiles.push_back(newProjectile);
}

void Game::CleanupInactiveEntities() noexcept
{
    std::erase_if(Projectiles, [](Projectile& projectile) noexcept { return !projectile.GetActive(); });
    std::erase_if(Aliens, [](Alien& alien) noexcept { return !alien.GetActive(); });
    std::erase_if(Walls, [](Wall& wall) noexcept { return !wall.GetActive(); });
}

void Game::ShowEndScreen() noexcept
{
    if (IsKeyReleased(KEY_ENTER) && !newHighScore)
    {
        Reset();
        gameState = State::STARTSCREEN;
    }

    if (newHighScore)
    {
        HandleHighScoreEntry();
    }
}

void Game::HandleHighScoreEntry() noexcept
{
    if (CheckCollisionPointRec(GetMousePosition(), textBox))
    {
        mouseOnText = true;
    }
    else
    {
        mouseOnText = false;
    }

    if (mouseOnText)
    {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        HandleNameInput();
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (mouseOnText)
    {
        framesCounter++;
    }
    else
    {
        framesCounter = 0;
    }

    if (letterCount > 0 && letterCount < MaxNameLength && IsKeyReleased(KEY_ENTER))
    {
        std::string nameEntry(name);
        InsertNewHighScore(nameEntry);
        newHighScore = false;
    }
}

void Game::HandleNameInput() noexcept
{
    int key = GetCharPressed();

    while (key > 0)
    {
        if ((key >= 32) && (key <= 125) && (letterCount < MaxNameLength))
        {
            name[letterCount] = (char)key;
            name[letterCount + 1] = '\0'; // Add null terminator
            letterCount++;
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        letterCount = std::max(letterCount - 1, 0);
        name[letterCount] = '\0';
    }
}

void Game::SpawnEnemyProjectile() noexcept
{
    if (Aliens.empty()) return;

    int const randomAlienIndex = (Aliens.size() > 1) ? rand() % Aliens.size() : 0;

    Projectile newProjectile(Aliens.at(randomAlienIndex).GetPosition(), EntityType::ENEMY_PROJECTILE);
    newProjectile.OffsetEnemyProjectile();
    newProjectile.InverseSpeed();

    Projectiles.push_back(std::move(newProjectile));
}

bool const Game::IsCollidingWith(const Projectile& projectile, const Entity& entity) noexcept
{
    return CheckCollision(entity.GetPosition(), entity.GetRadius(), projectile.GetLineStart(), projectile.GetLineEnd());
}

void Game::RenderNewHighScoreScreen() noexcept
{
    constexpr int textX = 600;
    DrawText("NEW HIGHSCORE!", textX, 300, 60, YELLOW);
    DrawText("PLACE MOUSE OVER INPUT BOX!", textX, 400, 20, YELLOW);

    DrawRectangleRec(textBox, LIGHTGRAY);
    DrawText(name, static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 8, 40, MAROON);
    DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, MaxNameLength), textX, 600, 20, YELLOW);

    RenderTextBoxOutline();
    RenderBlinkingCursor();

    if (letterCount > 0 && letterCount < MaxNameLength)
    {
        DrawText("PRESS ENTER TO CONTINUE", textX, 800, 40, YELLOW);
    }
}

void Game::RenderTextBoxOutline() noexcept
{
    const auto outlineColor = mouseOnText ? RED : DARKGRAY;
    DrawRectangleLines(static_cast<int>(textBox.x), static_cast<int>(textBox.y),
        static_cast<int>(textBox.width), static_cast<int>(textBox.height), outlineColor);
}

void Game::RenderBlinkingCursor() noexcept
{
    if (!mouseOnText || letterCount >= MaxNameLength)
    {
        if (letterCount >= MaxNameLength)
        {
            DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
        }
        return;
    }

    if ((framesCounter / 20) % 2 == 0)
    {
        const int cursorX = static_cast<int>(textBox.x) + 8 + MeasureText(name, 40);
        const int cursorY = static_cast<int>(textBox.y) + 12;
        DrawText("_", cursorX, cursorY, 40, MAROON);
    }
}

void Game::RenderLeaderboardScreen() noexcept
{
    DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);
    DrawText("LEADERBOARD", 50, 100, 40, YELLOW);

    int yOffset = 140;
    const int nameX = 50;
    const int scoreX = 350;
    const int rowHeight = 40;

    for (const auto& entry : Leaderboard)
    {
        DrawText(entry.name.c_str(), nameX, yOffset, 40, YELLOW);
        DrawText(TextFormat("%i", entry.score), scoreX, yOffset, 40, YELLOW);
        yOffset += rowHeight;
    }
}

void Game::Render() noexcept
{
    switch (gameState)
    {
    case State::STARTSCREEN:
        DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
        DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
        break;

    case State::GAMEPLAY:
        background.Render();
        DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
        DrawText(TextFormat("Lives: %i", player.GetLives()), 50, 70, 40, YELLOW);

        player.Render(resources);
        for (Projectile& projectile : Projectiles)
        {
            projectile.Render(resources.laserTexture->get());
        }
        for (Wall& wall : Walls)
        {
            wall.Render(resources.barrierTexture->get());
        }
        for (Alien& alien : Aliens)
        {
            alien.Render(resources.alienTexture->get());
        }
        break;

    case State::ENDSCREEN:
        if (newHighScore)
        {
            RenderNewHighScoreScreen();
        }
        else
        {
            RenderLeaderboardScreen();
        }
        break;

    default:
        break;
    }
}

void Game::SpawnAliens() noexcept
{
    for (int row = 0; row < formationHeight; ++row)
    {
        for (int col = 0; col < formationWidth; ++col)
        {
            const float xPos = static_cast<float>(formationX + col * alienSpacing);
            const float yPos = static_cast<float>(formationY + row * alienSpacing);
            Aliens.push_back(Alien({ xPos, yPos }));
        }
    }
}


bool Game::CheckNewHighScore() noexcept
{
	if (score > Leaderboard.at(4).score)
	{
		return true;
	}

	return false;
}

void Game::InsertNewHighScore(std::string newName) noexcept
{
	PlayerData newData;
	newData.name = newName;
	newData.score = score;

	for (int i = 0; i < Leaderboard.size(); i++)
	{
		if (newData.score > Leaderboard.at(i).score)
		{

			Leaderboard.insert(Leaderboard.begin() + i, newData);

			Leaderboard.pop_back();

			i = static_cast<int>(Leaderboard.size());

		}
	}
}