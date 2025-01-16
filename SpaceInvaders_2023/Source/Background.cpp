#include "Background.h"

Background::Background(int starAmount)
{
	for (int i = 0; i < starAmount; i++)
	{
		Star newStar({ 
			static_cast<float> (GetRandomValue(-150, GetScreenWidth() + 150)),
			static_cast<float>(GetRandomValue(0, GetScreenHeight())
			)});
		newStar.SetColor(SKYBLUE);
		newStar.SetSize(GetRandomValue(1, 4) / 2);
		Stars.push_back(newStar);

	}
}

void Background::Update(float offset) noexcept
{
	for (Star& star : Stars)
	{
		star.Update(offset);
	}
}

void Background::Render() const noexcept
{
	for (const Star& star : Stars)
	{
		star.Render();
	}
}