#pragma once
#include <raylib.h>
#include <math.h>
float lineLength(Vector2 A, Vector2 B) noexcept
{
	return sqrtf(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));
}

bool pointInCircle(Vector2 circlePos, float radius, Vector2 point) noexcept
{
		return lineLength(circlePos, point) <radius;
}