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
        float const window_width = static_cast<float>(GetScreenWidth());
        position.x = window_width / 2;
        position.y = static_cast<float>(GetScreenHeight()) - 50.0f;
    }

     Texture2D GetTexture(Resources& resource) const noexcept { return resource.shipTextures.at(textureFrame)->get(); }
     float GetPlayerBaseHeight() noexcept { return player_base_height; }
    int GetTextureFrame() noexcept { return textureFrame; }
    void Update() override;
    void Render(Resources& resource) const noexcept;
    void Render(Texture2D texture) const noexcept;
    int GetLives() const noexcept { return lives; }
    void DecreaseLife() noexcept { if (lives > 0) lives--; }

};
