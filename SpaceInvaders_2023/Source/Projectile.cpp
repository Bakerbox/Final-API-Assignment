#include "Projectile.h"
#define OutOfBoundsY 1500
void Projectile::Update() noexcept {

    position.y -= speed;


    lineStart.y = position.y - 15;
    lineEnd.y = position.y + 15;

    lineStart.x = position.x;
    lineEnd.x = position.x;

    if (position.y < 0 || position.y > OutOfBoundsY)
    {
        active = false;
    }
}

void Projectile::Render(Texture2D texture) const noexcept
{
	DrawTexturePro(texture,
		{
			0,
			0,
			176,
			176,
		},
		{
			position.x,
			position.y,
			50,
			50,
		}, { 25 , 25 },
		0,
		WHITE);
}
