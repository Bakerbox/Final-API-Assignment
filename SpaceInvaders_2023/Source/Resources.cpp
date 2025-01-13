#include "Resources.h"
#include <iostream>
#include <vector>
#include <memory> 




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