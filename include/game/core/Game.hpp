#pragma once

#include "game/assets/AssetManager.hpp"
#include "game/characters/Enemy.hpp"
#include "game/characters/Player.hpp"
#include "game/core/Input.hpp"
#include "game/level/Level.hpp"
#include "game/physics/PhysicsSystem.hpp"
#include "raylib.h"

#include <memory>
#include <vector>

namespace game {

class Game {
public:
    Game();
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void run();

private:
    void update(float dt);
    void draw() const;
    void resetWorld();
    void updateCamera();

    static constexpr int screenWidth_ = 1280;
    static constexpr int screenHeight_ = 720;

    Input input_;
    AssetManager assets_;
    PhysicsSystem physics_;
    Level level_;
    std::unique_ptr<Player> player_;
    std::vector<Enemy> enemies_;
    Camera2D camera_{};
    bool debugDraw_ = false;
};

} // namespace game
