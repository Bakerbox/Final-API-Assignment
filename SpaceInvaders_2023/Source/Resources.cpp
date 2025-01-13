#include "Resources.h"
#include <iostream>
#include <vector>
#include <memory> // For std::unique_ptr

// RAII wrapper for managing textures
class TextureResource {
private:
    Texture2D texture;

public:
    explicit TextureResource(const std::string& path) noexcept {
        texture = LoadTexture(path.c_str());
    }

    ~TextureResource() {
        UnloadTexture(texture);
    }

    TextureResource(const TextureResource&) = delete;
    TextureResource& operator=(const TextureResource&) = delete;


    TextureResource(TextureResource&& other) noexcept : texture(other.texture) {
        other.texture.id = 0;
    }
    TextureResource& operator=(TextureResource&& other) noexcept {
        if (this != &other) {
            UnloadTexture(texture);
            texture = other.texture;
            other.texture.id = 0;
        }
        return *this;
    }

    Texture get() const noexcept {
        return texture;
    }
};


Resources::Resources() noexcept
    : alienTexture(std::make_unique<TextureResource>("./Assets/Alien.png")),
      barrierTexture(std::make_unique<TextureResource>("./Assets/Barrier.png")),
      laserTexture(std::make_unique<TextureResource>("./Assets/Laser.png"))
{
    shipTextures.emplace_back(std::make_unique<TextureResource>("./Assets/Ship1.png"));
    shipTextures.emplace_back(std::make_unique<TextureResource>("./Assets/Ship2.png"));
    shipTextures.emplace_back(std::make_unique<TextureResource>("./Assets/Ship3.png"));
}

Resources::~Resources() noexcept = default; 