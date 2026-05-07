#pragma once

#include "raylib.h"

#include <string>
#include <unordered_map>

namespace game {

class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager();

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    Texture2D& loadTexture(const std::string& id, const std::string& path);
    Texture2D& getTexture(const std::string& id);
    bool hasTexture(const std::string& id) const;
    void unloadAll();

private:
    std::unordered_map<std::string, Texture2D> textures_;
};

} // namespace game
