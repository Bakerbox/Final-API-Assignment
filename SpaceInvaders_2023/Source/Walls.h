#pragma once
#include "Entity.h"

class Wall : public Entity {
    int health = 50;
    bool active = true;
    Color color = { 0,0,0,0 };
public:
    Wall(Vector2 pos) noexcept {
        radius = 60;
       position = pos;
    }
    bool GetActive() noexcept { return active; }
    void Update() override;
    void Render(Texture2D texture) const override;
    bool IsDestroyed() const noexcept { return health <= 0; }
    void TakeDamage() noexcept { if (health > 0) health--; }

};
