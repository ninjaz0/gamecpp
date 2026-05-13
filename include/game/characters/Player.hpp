#pragma once

#include "game/core/Input.hpp"
#include "game/level/Level.hpp"
#include "game/physics/PhysicsBody.hpp"
#include "game/physics/PhysicsSystem.hpp"
#include "raylib.h"

#include <string_view>

namespace game {

enum class PlayerState {
    Idle,
    Run,
    Dash,
    Jump,
    Fall,
    Attack,
    Hurt
};

class Player {
public:
    explicit Player(Vector2 spawnPosition);

    void update(float dt, const InputState& input, PhysicsSystem& physics, const Level& level);
    void draw(bool debugDraw) const;
    void reset(Vector2 position);
    void takeDamage(Vector2 knockback);

    Rectangle bounds() const;
    Vector2 center() const;
    PlayerState state() const { return state_; }

private:
    void chooseState(const InputState& input);
    Color colorForState() const;

    PhysicsBody body_;
    PlayerState state_ = PlayerState::Idle;
    int facing_ = 1;
    float attackTimer_ = 0.0f;
    float hurtTimer_ = 0.0f;
    float moveSpeed_ = 245.0f;
    float jumpSpeed_ = -610.0f;
    float dashSpeedMultiplier_ = 1.6f;      
    float accel_ = 10.0f;           
    float friction_ = 900.0f;
};

std::string_view toString(PlayerState state);

} // namespace game
