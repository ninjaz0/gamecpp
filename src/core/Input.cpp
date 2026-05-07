#include "game/core/Input.hpp"

#include "raylib.h"

#include <algorithm>
#include <cmath>

namespace game {

void Input::update() {
    state_ = InputState{};

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        state_.moveX -= 1.0f;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        state_.moveX += 1.0f;
    }

    state_.jumpPressed = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP);
    state_.attackPressed = IsKeyPressed(KEY_J) || IsKeyPressed(KEY_K);
    state_.resetPressed = IsKeyPressed(KEY_R);
    state_.debugTogglePressed = IsKeyPressed(KEY_F1);
    state_.dashHeld = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    if (IsGamepadAvailable(0)) {
        const float axis = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        if (std::abs(axis) > 0.22f) {
            state_.moveX = axis;
        }

        state_.jumpPressed = state_.jumpPressed || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
        state_.attackPressed = state_.attackPressed || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
        state_.dashHeld = state_.dashHeld || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1);
    }

    state_.moveX = std::clamp(state_.moveX, -1.0f, 1.0f);
}

} // namespace game
