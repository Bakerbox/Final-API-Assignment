#include "Walls.h"

void Wall::Update() {
    if (health < 1)
    {
        active = false;
    }
}

void Wall::Render(Texture2D texture) const {
    DrawTexturePro(texture,
        { 0, 0, 704, 704 },
        { position.x, position.y, 200, 200 },
        { 100, 100 },  
        0.0f,      
        WHITE);

    if (health > 0) {
        DrawText(TextFormat("%i", health), position.x - 10, position.y - 10, 20, RED);
    }
}
