#pragma once 
#include "Entity.h"

#define alienOffset 40
class Projectile : public Entity
{
private:
	int speed = 15;
	bool active = true;
	EntityType type = {};
	Vector2 lineStart = { 0, 0 };
	Vector2 lineEnd = { 0, 0 };
public:


	Projectile(Vector2 pos, EntityType typ) noexcept {
		radius = 0;
		position = pos;
		type = typ;
	}
	
	EntityType GetType() noexcept { return type; }
	Vector2  GetLineStart() const noexcept { return lineStart; }
	Vector2  GetLineEnd() const noexcept { return lineEnd; }
	bool const GetActive() noexcept { return active; }
	void SetActive(bool set) noexcept { active = set; }
	void InverseSpeed() noexcept { speed = -speed; }
	void OffsetEnemyProjectile() noexcept { position.y += alienOffset; };
	void Update() noexcept override;
	void Render(Texture2D texture) const noexcept override;


};