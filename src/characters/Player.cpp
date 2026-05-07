#include "game/characters/Player.hpp"

#include <cmath>

namespace game {

Player::Player(Vector2 spawnPosition) {
    reset(spawnPosition);
}

void Player::update(float dt, const InputState& input, PhysicsSystem& physics, const Level& level) {
    if (input.moveX != 0.0f) {
        facing_ = input.moveX > 0.0f ? 1 : -1;
    }

    if (hurtTimer_ > 0.0f) {
        hurtTimer_ -= dt;
    } else {
        const float speedMultiplier = input.dashHeld ? dashSpeedMultiplier_ : 1.0f;
        body_.velocity.x = input.moveX * moveSpeed_ * speedMultiplier;

        if (input.jumpPressed && body_.grounded) {
            body_.velocity.y = jumpSpeed_;
            body_.grounded = false;
        }

        if (input.attackPressed && attackTimer_ <= 0.0f) {
            attackTimer_ = 0.22f;
        }
    }

    if (attackTimer_ > 0.0f) {
        attackTimer_ -= dt;
        body_.velocity.x *= 0.55f;
    }

    physics.step(body_, level.solids(), dt);
    chooseState(input);
}

void Player::draw(bool debugDraw) const {
    const Rectangle rect = body_.bounds();
    const Color bodyColor = colorForState();

    DrawRectangleRounded(rect, 0.18f, 8, bodyColor);
    DrawRectangle(static_cast<int>(rect.x + (facing_ > 0 ? rect.width - 9.0f : 4.0f)),
                  static_cast<int>(rect.y + 12.0f),
                  5,
                  7,
                  RAYWHITE);

    if (state_ == PlayerState::Attack) {
        const Rectangle attackBox{
            facing_ > 0 ? rect.x + rect.width : rect.x - 30.0f,
            rect.y + 12.0f,
            30.0f,
            18.0f
        };
        DrawRectangleRec(attackBox, Color{244, 210, 92, 150});
    }

    if (debugDraw) {
        DrawRectangleLinesEx(rect, 2.0f, GREEN);
        DrawLineV(center(), Vector2{center().x + body_.velocity.x * 0.12f, center().y + body_.velocity.y * 0.12f}, SKYBLUE);
    }
}

void Player::reset(Vector2 position) {
    body_.position = position;
    body_.size = Vector2{32.0f, 54.0f};
    body_.velocity = Vector2{};
    body_.grounded = false;
    state_ = PlayerState::Idle;
    attackTimer_ = 0.0f;
    hurtTimer_ = 0.0f;
    facing_ = 1;
}

void Player::takeDamage(Vector2 knockback) {
    body_.velocity = knockback;
    body_.grounded = false;
    hurtTimer_ = 0.35f;
    attackTimer_ = 0.0f;
    state_ = PlayerState::Hurt;
}

Rectangle Player::bounds() const {
    return body_.bounds();
}

Vector2 Player::center() const {
    return Vector2{
        body_.position.x + body_.size.x * 0.5f,
        body_.position.y + body_.size.y * 0.5f
    };
}

void Player::chooseState(const InputState& input) {
    if (hurtTimer_ > 0.0f) {
        state_ = PlayerState::Hurt;
        return;
    }

    if (attackTimer_ > 0.0f) {
        state_ = PlayerState::Attack;
        return;
    }

    if (!body_.grounded) {
        state_ = body_.velocity.y < 0.0f ? PlayerState::Jump : PlayerState::Fall;
        return;
    }
    if (input.dashHeld && std::abs(input.moveX) > 0.01f) {
        state_ = PlayerState::Dash;
        return;
    }

    state_ = std::abs(input.moveX) > 0.01f ? PlayerState::Run : PlayerState::Idle;
}

Color Player::colorForState() const {
    switch (state_) {
        case PlayerState::Idle: return Color{70, 130, 210, 255};
        case PlayerState::Run: return Color{74, 170, 130, 255};
        case PlayerState::Jump: return Color{110, 120, 230, 255};
        case PlayerState::Fall: return Color{150, 105, 210, 255};
        case PlayerState::Attack: return Color{235, 186, 76, 255};
        case PlayerState::Hurt: return Color{220, 75, 80, 255};
        case PlayerState::Dash: return Color{255, 215, 0, 255};
    }

    return WHITE;
}

std::string_view toString(PlayerState state) {
    switch (state) {
        case PlayerState::Idle: return "Idle";
        case PlayerState::Run: return "Run";
        case PlayerState::Jump: return "Jump";
        case PlayerState::Fall: return "Fall";
        case PlayerState::Attack: return "Attack";
        case PlayerState::Hurt: return "Hurt";
        case PlayerState::Dash: return "Dash";
    }

    return "Unknown";
}

} // namespace game
