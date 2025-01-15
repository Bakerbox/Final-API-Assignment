#pragma once
#include "Entity.h"

class Alien : public Entity {
    float speed = 2;
    bool moveRight = true;
    bool active = true;
    EntityType type = EntityType::ENEMY;


public:
    explicit Alien(Vector2 pos) noexcept
         {
        position = pos; radius = 30.0f;
    }

    bool GetActive() noexcept { return active; }
    void SetActive(bool set) noexcept { active = set; }
    void Update() override;
    void Render(Texture2D texture) const override;
};
