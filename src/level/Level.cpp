#include "game/level/Level.hpp"

#include "raylib.h"

#include <algorithm>
#include <fstream>
#include <string>

namespace game {

bool Level::loadFromFile(const std::string& path) {
    if (path.empty()) {
        return false;
    }

    std::ifstream file(path);
    if (!file) {
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }

    if (lines.empty()) {
        return false;
    }

    parseLines(lines);
    return true;
}

void Level::loadFallback() {
    parseLines({
        "................................",
        "................................",
        ".....P..........................",
        ".....................E..........",
        "..............#####.............",
        "................................",
        "####.................#####......",
        "################################"
    });
}

void Level::draw(bool debugDraw) const {
    const int worldWidth = widthInTiles_ * static_cast<int>(tileSize_);
    const int worldHeight = heightInTiles_ * static_cast<int>(tileSize_);

    DrawRectangleGradientV(0, 0, worldWidth, worldHeight, Color{32, 38, 54, 255}, Color{17, 20, 28, 255});

    for (const Rectangle& solid : solids_) {
        DrawRectangleRec(solid, Color{57, 67, 83, 255});
        DrawRectangle(static_cast<int>(solid.x), static_cast<int>(solid.y), static_cast<int>(solid.width), 4, Color{82, 98, 118, 255});
        if (debugDraw) {
            DrawRectangleLinesEx(solid, 1.0f, Color{244, 210, 92, 255});
        }
    }
}

bool Level::overlapsSolid(Rectangle area) const {
    return std::ranges::any_of(solids_, [area](const Rectangle& solid) {
        return CheckCollisionRecs(area, solid);
    });
}

void Level::parseLines(const std::vector<std::string>& lines) {
    solids_.clear();
    data_ = LevelData{};
    widthInTiles_ = 0;
    heightInTiles_ = static_cast<int>(lines.size());

    for (const std::string& line : lines) {
        widthInTiles_ = std::max(widthInTiles_, static_cast<int>(line.size()));
    }

    for (int y = 0; y < static_cast<int>(lines.size()); ++y) {
        for (int x = 0; x < static_cast<int>(lines[y].size()); ++x) {
            const char tile = lines[y][x];
            const Vector2 position{
                static_cast<float>(x) * tileSize_,
                static_cast<float>(y) * tileSize_
            };

            if (tile == '#') {
                solids_.push_back(Rectangle{position.x, position.y, tileSize_, tileSize_});
            } else if (tile == 'P') {
                data_.playerSpawn = Vector2{position.x + 4.0f, position.y - 16.0f};
            } else if (tile == 'E') {
                data_.enemySpawns.push_back(Vector2{position.x + 3.0f, position.y - 2.0f});
            }
        }
    }
}

} // namespace game
