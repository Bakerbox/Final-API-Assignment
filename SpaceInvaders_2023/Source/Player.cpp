#include "Player.h"
#include <algorithm>

void Player::Update() {
    int direction = 0;
    if (IsKeyDown(KEY_LEFT)) direction -= 1;
    if (IsKeyDown(KEY_RIGHT)) direction += 1;

    position.x += speed * direction;
    position.x = std::clamp(position.x, radius, (float)GetScreenWidth() - radius);

    frameTimer += GetFrameTime();
    if (frameTimer > 0.4f) {
        textureFrame = (textureFrame + 1) % 3;  // Cycle through frames
        frameTimer = 0.0f;
    }
}

void Player::Render(Resources& resource) const noexcept {
    const float height = static_cast<float>(GetScreenHeight());
    DrawTexturePro(GetTexture(resource),
        { textureFrame * 352.0f, 0, 352, 352 },       // Source rectangle
        { position.x, height - 50, 50, 50 }, // Destination rectangle
        { 25, 25 },                                    // Origin
        0.0f,                                        // Rotation
        WHITE);
}

void Player::Render(Texture2D texture) const noexcept {
    
}
