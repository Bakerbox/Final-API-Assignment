#pragma once
#include <raylib.h>

enum struct EntityType
{
    PLAYER,
    ENEMY,
    PLAYER_PROJECTILE,
    ENEMY_PROJECTILE
};
class Entity {
protected:
    Vector2 position;
    float radius;

public:
    explicit  Entity() noexcept { radius = 0; }
    virtual ~Entity() = default;

    virtual void Update() = 0;
    virtual void Render(Texture2D texture) const = 0;

    Vector2 GetPosition() const noexcept { return position; }
    float GetXPosition() const noexcept { return position.x; }
    float GetYPosition() const noexcept { return position.y; }
    float GetRadius() const noexcept { return radius; }
    void SetPosition(Vector2 newPos) noexcept { position = newPos; }

    bool IsColliding(const Entity& other) const noexcept {
        return CheckCollisionCircles(position, radius, other.position, other.radius);
    }
};
