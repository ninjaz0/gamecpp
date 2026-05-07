#pragma once

#include "game/level/Level.hpp"
#include "game/physics/PhysicsBody.hpp"
#include "game/physics/PhysicsSystem.hpp"
#include "raylib.h"

namespace game {

class Enemy {
public:
    explicit Enemy(Vector2 spawnPosition);

    void update(float dt, PhysicsSystem& physics, const Level& level);
    void draw(bool debugDraw) const;

    Rectangle bounds() const;

private:
    PhysicsBody body_;
    int direction_ = -1;
    float patrolSpeed_ = 95.0f;
};

} // namespace game
