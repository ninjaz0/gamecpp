#pragma once

namespace game {

struct InputState {
    float moveX = 0.0f;
    bool jumpPressed = false;
    bool attackPressed = false;
    bool resetPressed = false;
    bool debugTogglePressed = false;
};

class Input {
public:
    void update();
    const InputState& state() const { return state_; }

private:
    InputState state_{};
};

} // namespace game
