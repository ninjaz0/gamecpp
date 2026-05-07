#include "game/physics/PhysicsSystem.hpp"

#include <algorithm>

namespace game {

CollisionInfo PhysicsSystem::step(PhysicsBody& body, const std::vector<Rectangle>& solids, float dt) const {
    CollisionInfo collision{};

    body.velocity.y += gravity * body.gravityScale * dt;
    body.velocity.y = std::min(body.velocity.y, maxFallSpeed);
    body.grounded = false;

    body.position.x += body.velocity.x * dt;
    for (const Rectangle& solid : solids) {
        if (!CheckCollisionRecs(body.bounds(), solid)) {
            continue;
        }

        if (body.velocity.x > 0.0f) {
            body.position.x = solid.x - body.size.x;
            collision.hitRight = true;
        } else if (body.velocity.x < 0.0f) {
            body.position.x = solid.x + solid.width;
            collision.hitLeft = true;
        }
        body.velocity.x = 0.0f;
    }

    body.position.y += body.velocity.y * dt;
    for (const Rectangle& solid : solids) {
        if (!CheckCollisionRecs(body.bounds(), solid)) {
            continue;
        }

        if (body.velocity.y > 0.0f) {
            body.position.y = solid.y - body.size.y;
            body.grounded = true;
            collision.hitBottom = true;
        } else if (body.velocity.y < 0.0f) {
            body.position.y = solid.y + solid.height;
            collision.hitTop = true;
        }
        body.velocity.y = 0.0f;
    }

    return collision;
}

} // namespace game
