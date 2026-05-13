#include "game/core/Game.hpp"

#include <algorithm>
#include <string>

namespace game {

namespace {

std::string findLevelPath() {
    const char* candidates[] = {
        "assets/levels/demo.txt",
        "../assets/levels/demo.txt",
        "../../assets/levels/demo.txt"
    };

    for (const char* candidate : candidates) {
        if (FileExists(candidate)) {
            return candidate;
        }
    }

    return {};
}

} // namespace

Game::Game() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth_, screenHeight_, "HollowStarter");
    SetExitKey(KEY_NULL);

    camera_.offset = Vector2{screenWidth_ * 0.5f, screenHeight_ * 0.5f};
    camera_.target = Vector2{};
    camera_.rotation = 0.0f;
    camera_.zoom = 1.0f;

    resetWorld();
}

Game::~Game() {
    assets_.unloadAll();
    CloseWindow();
}

void Game::run() {
    while (!WindowShouldClose()) {
        const float dt = std::min(GetFrameTime(), 1.0f / 20.0f);
        input_.update();
        update(dt);
        draw();
    }
}

void Game::update(float dt) {
    const InputState& input = input_.state();

    if (input.debugTogglePressed) {
        debugDraw_ = !debugDraw_;
    }

    if (input.resetPressed) {
        resetWorld();
        return;
    }

    player_->update(dt, input, physics_, level_);

    for (Enemy& enemy : enemies_) {
        enemy.update(dt, physics_, level_);

        if (!enemy.isDead() && player_->isAttacking() && CheckCollisionRecs(player_->attackBounds(), enemy.bounds())) {
            const float knockbackX = player_->center().x < enemy.bounds().x ? 260.0f : -260.0f;
            enemy.takeDamage(player_->attackId(), Vector2{knockbackX, -170.0f});
        }

        if (!enemy.isDead() && CheckCollisionRecs(player_->bounds(), enemy.bounds())) {
            const float knockbackX = player_->center().x < enemy.bounds().x ? -360.0f : 360.0f;
            player_->takeDamage(Vector2{knockbackX, -360.0f});
        }
    }

    std::erase_if(enemies_, [](const Enemy& enemy) {
        return enemy.isDead();
    });

    updateCamera();
}

void Game::draw() const {
    BeginDrawing();
    ClearBackground(Color{18, 22, 30, 255});

    BeginMode2D(camera_);
    level_.draw(debugDraw_);
    for (const Enemy& enemy : enemies_) {
        enemy.draw(debugDraw_);
    }
    player_->draw(debugDraw_);
    EndMode2D();

    DrawRectangle(24, 22, 178, 18, Color{44, 52, 68, 255});
    DrawRectangle(28, 26, 122, 10, Color{93, 201, 139, 255});
    DrawText(TextFormat("State: %s", toString(player_->state()).data()), 24, 50, 20, Color{220, 226, 236, 255});

    if (debugDraw_) {
        DrawText("Debug AABB", 24, 78, 18, Color{244, 210, 92, 255});
    }

    EndDrawing();
}

void Game::resetWorld() {
    if (!level_.loadFromFile(findLevelPath())) {
        level_.loadFallback();
    }

    player_ = std::make_unique<Player>(level_.data().playerSpawn);
    enemies_.clear();
    for (Vector2 spawn : level_.data().enemySpawns) {
        enemies_.emplace_back(spawn);
    }

    updateCamera();
}

void Game::updateCamera() {
    camera_.offset = Vector2{GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f};

    Vector2 target = player_->center();
    target.y -= 70.0f;

    camera_.target.x += (target.x - camera_.target.x) * 0.12f;
    camera_.target.y += (target.y - camera_.target.y) * 0.12f;
}

} // namespace game
