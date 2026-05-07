#pragma once

#include "game/physics/AABB.hpp"
#include "raylib.h"

namespace game {

struct PhysicsBody {
    Vector2 position{};
    Vector2 size{32.0f, 48.0f};
    Vector2 velocity{};
    bool grounded = false;
    float gravityScale = 1.0f;

    Rectangle bounds() const {
        return AABB{position, size}.toRectangle();
    }
};

} // namespace game
