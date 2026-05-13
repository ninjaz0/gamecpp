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

    if (gameWon_ || gameOver_) {
        updateCamera();
        return;
    }

    player_->update(dt, input, physics_, level_);
    updateCollectibles();
    updateHazards();

    for (Enemy& enemy : enemies_) {
        enemy.update(dt, physics_, level_);

        if (!enemy.isDead() && player_->isAttacking() && CheckCollisionRecs(player_->attackBounds(), enemy.bounds())) {
            const float knockbackX = player_->center().x < enemy.bounds().x ? 260.0f : -260.0f;
            enemy.takeDamage(player_->attackId(), Vector2{knockbackX, -170.0f});
        }

        if (!enemy.isDead() && CheckCollisionRecs(player_->bounds(), enemy.bounds())) {
            const float knockbackX = player_->center().x < enemy.bounds().x ? -360.0f : 360.0f;
            player_->takeDamage(Vector2{knockbackX, -360.0f});
            gameOver_ = player_->isDead();
        }
    }

    std::erase_if(enemies_, [](const Enemy& enemy) {
        return enemy.isDead();
    });

    updateGoal();
    updateCamera();
}

void Game::draw() const {
    BeginDrawing();
    ClearBackground(Color{18, 22, 30, 255});

    BeginMode2D(camera_);
    level_.draw(debugDraw_);
    drawCollectibles();
    for (const Enemy& enemy : enemies_) {
        enemy.draw(debugDraw_);
    }
    player_->draw(debugDraw_);
    EndMode2D();

    drawHud();

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
    collectibles_.clear();
    collectibles_.reserve(level_.data().collectibles.size());
    for (Rectangle collectible : level_.data().collectibles) {
        collectibles_.push_back(Collectible{collectible, false});
    }
    collectedCount_ = 0;
    gameWon_ = false;
    gameOver_ = false;

    updateCamera();
}

void Game::updateCamera() {
    camera_.offset = Vector2{GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f};

    Vector2 target = player_->center();
    target.y -= 70.0f;

    camera_.target.x += (target.x - camera_.target.x) * 0.12f;
    camera_.target.y += (target.y - camera_.target.y) * 0.12f;
}

void Game::updateCollectibles() {
    for (Collectible& collectible : collectibles_) {
        if (!collectible.collected && CheckCollisionRecs(player_->bounds(), collectible.bounds)) {
            collectible.collected = true;
            ++collectedCount_;
        }
    }
}

void Game::updateHazards() {
    for (const Rectangle& hazard : level_.data().hazards) {
        if (CheckCollisionRecs(player_->bounds(), hazard)) {
            const float knockbackX = player_->center().x < hazard.x + hazard.width * 0.5f ? -250.0f : 250.0f;
            player_->takeDamage(Vector2{knockbackX, -420.0f});
            gameOver_ = player_->isDead();
            return;
        }
    }
}

void Game::updateGoal() {
    if (!level_.data().hasGoal || collectedCount_ < static_cast<int>(collectibles_.size())) {
        return;
    }

    gameWon_ = CheckCollisionRecs(player_->bounds(), level_.data().goal);
}

void Game::drawCollectibles() const {
    for (const Collectible& collectible : collectibles_) {
        if (collectible.collected) {
            continue;
        }

        const Vector2 center{
            collectible.bounds.x + collectible.bounds.width * 0.5f,
            collectible.bounds.y + collectible.bounds.height * 0.5f
        };
        DrawCircleV(center, 10.0f, Color{91, 205, 250, 255});
        DrawCircleV(center, 5.0f, Color{232, 246, 255, 255});
        if (debugDraw_) {
            DrawRectangleLinesEx(collectible.bounds, 1.0f, Color{91, 205, 250, 255});
        }
    }
}

void Game::drawHud() const {
    DrawRectangle(18, 18, 330, 100, Color{18, 22, 30, 210});
    for (int i = 0; i < player_->maxHealth(); ++i) {
        const Color heartColor = i < player_->health() ? Color{220, 75, 80, 255} : Color{78, 86, 102, 255};
        DrawRectangleRounded(Rectangle{28.0f + i * 30.0f, 28.0f, 22.0f, 18.0f}, 0.35f, 6, heartColor);
    }

    DrawText(TextFormat("Crystals: %d/%d", collectedCount_, static_cast<int>(collectibles_.size())),
             28,
             56,
             20,
             Color{220, 226, 236, 255});
    DrawText(TextFormat("State: %s", toString(player_->state()).data()), 28, 82, 18, Color{172, 184, 202, 255});

    if (debugDraw_) {
        DrawText("Debug AABB", 210, 84, 16, Color{244, 210, 92, 255});
    }

    if (gameWon_ || gameOver_) {
        const char* title = gameWon_ ? "LEVEL CLEAR" : "GAME OVER";
        const char* prompt = "Press R to restart";
        const int titleWidth = MeasureText(title, 42);
        const int promptWidth = MeasureText(prompt, 22);
        const int x = GetScreenWidth() / 2;
        const int y = GetScreenHeight() / 2;

        DrawRectangle(x - 210, y - 72, 420, 144, Color{18, 22, 30, 225});
        DrawText(title, x - titleWidth / 2, y - 44, 42, gameWon_ ? Color{92, 220, 160, 255} : Color{220, 75, 80, 255});
        DrawText(prompt, x - promptWidth / 2, y + 22, 22, Color{220, 226, 236, 255});
    }
}

} // namespace game
