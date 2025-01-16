#pragma once
#include "Entity.h"
#include "Star.h"
#include <vector>

class Background
{
	std::vector<Star> Stars;
public:
	Background(int starAmount);
	void Update(float offset) noexcept;
	void Render() const noexcept;

};