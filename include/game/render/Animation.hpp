#pragma once

#include "raylib.h"

#include <string>
#include <unordered_map>

namespace game {

struct AnimationClip {
    int firstFrame = 0;
    int frameCount = 1;
    float frameTime = 0.1f;
    bool loop = true;
};

class AnimationPlayer {
public:
    void setSpriteSheet(const Texture2D& texture, int frameWidth, int frameHeight);
    void addClip(const std::string& name, AnimationClip clip);
    void play(const std::string& name);
    void update(float dt);
    void draw(Vector2 position, float scale, bool flipX, Color tint) const;

    bool hasSpriteSheet() const { return texture_ != nullptr; }
    const std::string& currentClip() const { return currentClip_; }

private:
    Rectangle sourceRectangle() const;

    const Texture2D* texture_ = nullptr;
    int frameWidth_ = 0;
    int frameHeight_ = 0;
    int currentFrame_ = 0;
    float elapsed_ = 0.0f;
    std::string currentClip_;
    std::unordered_map<std::string, AnimationClip> clips_;
};

} // namespace game
