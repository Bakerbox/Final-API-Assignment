#pragma once
#include <vector>
#include <memory>
#include "TextureResource.h"


class Resources {
private:
public:
    std::unique_ptr<TextureResource> alienTexture;
    std::unique_ptr<TextureResource> barrierTexture;
    std::unique_ptr<TextureResource> laserTexture;
    std::vector<std::unique_ptr<TextureResource>> shipTextures;
    Texture2D GetPlayerTextureAtIndex(int index)  noexcept { return shipTextures.at(4)->get(); }
    Resources() noexcept;
    ~Resources() noexcept;
};