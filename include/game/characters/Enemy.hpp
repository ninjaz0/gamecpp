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
    bool takeDamage(int attackId, Vector2 knockback);

    Rectangle bounds() const;
    bool isDead() const { return health_ <= 0; }

private:
    PhysicsBody body_;
    int direction_ = -1;
    int health_ = 3;
    int lastHitAttackId_ = -1;
    float hurtTimer_ = 0.0f;
    float patrolSpeed_ = 95.0f;
};

} // namespace game
