#pragma once

#include "raylib.h"

namespace game {

struct AABB {
    Vector2 position{};
    Vector2 size{};

    Rectangle toRectangle() const {
        return Rectangle{position.x, position.y, size.x, size.y};
    }

    bool intersects(const AABB& other) const {
        return CheckCollisionRecs(toRectangle(), other.toRectangle());
    }
};

} // namespace game
