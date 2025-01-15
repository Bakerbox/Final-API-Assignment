#include "Alien.h"

void Alien::Update() {
    if (moveRight) {
        position.x += speed;
        if (position.x >= GetScreenWidth() - radius) {
            moveRight = false;
            position.y += 50;  // Move down
        }
    }
    else {
        position.x -= speed;
        if (position.x <= radius) {
            moveRight = true;
            position.y += 50;  // Move down
        }
    }
}

void Alien::Render(Texture2D texture) const
{

	DrawTexturePro(texture,
		{
			0,
			0,
			352,
			352,
		},
		{
			position.x,
			position.y,
			100,
			100,
		}, { 50 , 50 },
		0,
		WHITE);
}
