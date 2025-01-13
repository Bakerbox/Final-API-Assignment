#pragma once
#include "raylib.h"
#include <string>
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