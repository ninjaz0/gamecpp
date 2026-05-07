#include "game/characters/Enemy.hpp"

#include "raylib.h"

#include <algorithm>

namespace game {

Enemy::Enemy(Vector2 spawnPosition) {
    body_.position = spawnPosition;
    body_.size = Vector2{34.0f, 42.0f};
}

void Enemy::update(float dt, PhysicsSystem& physics, const Level& level) {
    hurtTimer_ = std::max(hurtTimer_ - dt, 0.0f);

    if (isDead()) {
        return;
    }

    body_.velocity.x = static_cast<float>(direction_) * patrolSpeed_;

    const CollisionInfo collision = physics.step(body_, level.solids(), dt);
    if ((collision.hitLeft && direction_ < 0) || (collision.hitRight && direction_ > 0)) {
        direction_ *= -1;
    }

    Rectangle footProbe{
        body_.position.x + (direction_ > 0 ? body_.size.x : -6.0f),
        body_.position.y + body_.size.y + 4.0f,
        6.0f,
        8.0f
    };

    if (body_.grounded && !level.overlapsSolid(footProbe)) {
        direction_ *= -1;
    }
}

void Enemy::draw(bool debugDraw) const {
    const Rectangle rect = body_.bounds();
    const Color bodyColor = hurtTimer_ > 0.0f ? Color{255, 232, 118, 255} : Color{204, 72, 82, 255};
    DrawRectangleRounded(rect, 0.18f, 6, bodyColor);
    DrawRectangle(static_cast<int>(rect.x + 8.0f), static_cast<int>(rect.y + 10.0f), 5, 5, RAYWHITE);
    DrawRectangle(static_cast<int>(rect.x + rect.width - 13.0f), static_cast<int>(rect.y + 10.0f), 5, 5, RAYWHITE);
    DrawText(TextFormat("%d", health_),
             static_cast<int>(rect.x + rect.width * 0.5f - 4.0f),
             static_cast<int>(rect.y - 18.0f),
             14,
             Color{235, 240, 248, 255});

    if (debugDraw) {
        DrawRectangleLinesEx(rect, 2.0f, RED);
    }
}

bool Enemy::takeDamage(int attackId, Vector2 knockback) {
    if (isDead() || lastHitAttackId_ == attackId) {
        return false;
    }

    lastHitAttackId_ = attackId;
    --health_;
    hurtTimer_ = 0.18f;
    body_.velocity = knockback;
    body_.grounded = false;
    direction_ = knockback.x < 0.0f ? 1 : -1;
    return true;
}

Rectangle Enemy::bounds() const {
    return body_.bounds();
}

} // namespace game
