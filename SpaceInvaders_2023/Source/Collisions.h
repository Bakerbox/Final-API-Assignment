#pragma once
#include "raylib.h"
#include "MathTools.h"

bool inline const CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd) noexcept
{
	if (pointInCircle(circlePos, circleRadius, lineStart) || pointInCircle(circlePos, circleRadius, lineEnd))
	{
		return true;
	}
	Vector2 const A = lineStart;
	Vector2 const B = lineEnd;
	Vector2 const C = circlePos;

	float const length = lineLength(A, B);
	float const dotP = (((C.x - A.x) * (B.x - A.x)) + ((C.y - A.y) * (B.y - A.y))) / pow(length, 2);
	float const closestX = A.x + (dotP * (B.x - A.x));
	float const closestY = A.y + (dotP * (B.y - A.y));
	float constexpr buffer = 0.1;

	float const closeToStart = lineLength(A, { closestX, closestY });
	float const closeToEnd = lineLength(B, { closestX, closestY });	

	float const closestLength = closeToStart + closeToEnd;

	if (closestLength == length + buffer || closestLength == length - buffer)
	{

		float const closeToCentre = lineLength(A, { closestX, closestY }); 
		if (closeToCentre < circleRadius)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}
