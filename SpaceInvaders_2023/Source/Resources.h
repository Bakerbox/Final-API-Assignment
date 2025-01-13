#pragma once
#include <vector>
#include <memory>
#include <raylib.h>

class TextureResource;

class Resources {
private:
    std::unique_ptr<TextureResource> alienTexture;
    std::unique_ptr<TextureResource> barrierTexture;
    std::unique_ptr<TextureResource> laserTexture;
    std::vector<std::unique_ptr<TextureResource>> shipTextures;

public:
    Resources() noexcept;
    ~Resources() noexcept;
};