#pragma once
#include <math.h>
#include <raylib.h>

	inline float const lineLength(Vector2 A, Vector2 B) noexcept
	{
		return sqrtf(static_cast<float>(pow(B.x - A.x, 2) + pow(B.y - A.y, 2)));
	}

	inline bool const pointInCircle(Vector2 circlePos, float radius, Vector2 point) noexcept
	{
		return lineLength(circlePos, point) < radius;
	}
