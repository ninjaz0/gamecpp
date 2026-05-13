#pragma once

#include "raylib.h"

#include <string>
#include <vector>

namespace game {

struct LevelData {
    Vector2 playerSpawn{96.0f, 96.0f};
    std::vector<Vector2> enemySpawns;
    std::vector<Rectangle> collectibles;
    std::vector<Rectangle> hazards;
    Rectangle goal{};
    bool hasGoal = false;
};

class Level {
public:
    bool loadFromFile(const std::string& path);
    void loadFallback();
    void draw(bool debugDraw) const;

    const std::vector<Rectangle>& solids() const { return solids_; }
    const LevelData& data() const { return data_; }
    bool overlapsSolid(Rectangle area) const;
    float tileSize() const { return tileSize_; }
    int widthInTiles() const { return widthInTiles_; }
    int heightInTiles() const { return heightInTiles_; }

private:
    void parseLines(const std::vector<std::string>& lines);

    float tileSize_ = 40.0f;
    int widthInTiles_ = 0;
    int heightInTiles_ = 0;
    std::vector<Rectangle> solids_;
    LevelData data_;
};

} // namespace game
