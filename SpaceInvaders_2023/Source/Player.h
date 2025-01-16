#pragma once
#include "Entity.h"
#include "Resources.h"
class Player : public Entity {
    int lives = 3;
    float speed = 7.0f;
    float frameTimer = 0.0f;
    EntityType type = EntityType::PLAYER;
    float player_base_height = 70.0f;
    int textureFrame = 0;

public:
     Player() noexcept
       {
        radius = 50; 
        float const window_width = GetScreenWidth();
        position.x = window_width / 2;
        position.y = GetScreenHeight() - 50;
    }

     Texture2D GetTexture(Resources& resource) const noexcept { return resource.shipTextures[textureFrame]->get(); }
     float GetPlayerBaseHeight() noexcept { return player_base_height; }
    int GetTextureFrame() noexcept { return textureFrame; }
    void Update() override;
    void Render(Resources& resource) const noexcept;
    void Render(Texture2D texture) const noexcept override;
    int GetLives() const noexcept { return lives; }
    void DecreaseLife() noexcept { if (lives > 0) lives--; }

};
