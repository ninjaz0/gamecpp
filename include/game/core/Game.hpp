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
    struct Collectible {
        Rectangle bounds{};
        bool collected = false;
    };

    void update(float dt);
    void draw() const;
    void resetWorld();
    void updateCamera();
    void updateCollectibles();
    void updateHazards();
    void updateGoal();
    void drawCollectibles() const;
    void drawHud() const;

    static constexpr int screenWidth_ = 1280;
    static constexpr int screenHeight_ = 720;

    Input input_;
    AssetManager assets_;
    PhysicsSystem physics_;
    Level level_;
    std::unique_ptr<Player> player_;
    std::vector<Enemy> enemies_;
    std::vector<Collectible> collectibles_;
    Camera2D camera_{};
    int collectedCount_ = 0;
    bool debugDraw_ = false;
    bool gameWon_ = false;
    bool gameOver_ = false;
};

} // namespace game
