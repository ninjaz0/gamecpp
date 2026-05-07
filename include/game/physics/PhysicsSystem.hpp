#pragma once

#include "game/physics/PhysicsBody.hpp"
#include "raylib.h"

#include <vector>

namespace game {

struct CollisionInfo {
    bool hitLeft = false;
    bool hitRight = false;
    bool hitTop = false;
    bool hitBottom = false;
};

class PhysicsSystem {
public:
    CollisionInfo step(PhysicsBody& body, const std::vector<Rectangle>& solids, float dt) const;

    float gravity = 1650.0f;
    float maxFallSpeed = 900.0f;
};

} // namespace game
