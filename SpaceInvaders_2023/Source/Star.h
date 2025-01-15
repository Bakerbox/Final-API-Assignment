#pragma once
#include "Entity.h"

class Star  {
    bool active = true;
    Color color = GRAY;
    float size = 0;
    Vector2 initPosition;
    Vector2 position;
public:
    Star(Vector2 pos) noexcept {
        position = pos;
        initPosition = pos;
    }
    void SetSize(float set) noexcept { size = set; }
    void SetColor(Color set) noexcept { color = set; }
    void Update(float starOffset) noexcept;
    void Render() const noexcept;
};
