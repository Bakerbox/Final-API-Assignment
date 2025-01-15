#include "Star.h"

void Star::Update(float starOffset) noexcept 
{
	position.x = initPosition.x + starOffset;
	position.y = initPosition.y;
}


void Star::Render() const noexcept 
{
	DrawCircle(static_cast<int>(position.x), static_cast<int>(position.y) ,size, color);
}