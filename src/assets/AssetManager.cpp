#include "game/assets/AssetManager.hpp"

#include <stdexcept>

namespace game {

AssetManager::~AssetManager() {
    unloadAll();
}

Texture2D& AssetManager::loadTexture(const std::string& id, const std::string& path) {
    if (auto it = textures_.find(id); it != textures_.end()) {
        return it->second;
    }

    Texture2D texture = LoadTexture(path.c_str());
    if (texture.id == 0) {
        throw std::runtime_error("Failed to load texture: " + path);
    }

    auto [it, inserted] = textures_.emplace(id, texture);
    return it->second;
}

Texture2D& AssetManager::getTexture(const std::string& id) {
    if (auto it = textures_.find(id); it != textures_.end()) {
        return it->second;
    }

    throw std::runtime_error("Texture not loaded: " + id);
}

bool AssetManager::hasTexture(const std::string& id) const {
    return textures_.contains(id);
}

void AssetManager::unloadAll() {
    for (auto& [id, texture] : textures_) {
        if (texture.id != 0) {
            UnloadTexture(texture);
        }
    }
    textures_.clear();
}

} // namespace game
