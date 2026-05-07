#include "game/render/Animation.hpp"

#include <algorithm>
#include <cmath>

namespace game {

void AnimationPlayer::setSpriteSheet(const Texture2D& texture, int frameWidth, int frameHeight) {
    texture_ = &texture;
    frameWidth_ = frameWidth;
    frameHeight_ = frameHeight;
}

void AnimationPlayer::addClip(const std::string& name, AnimationClip clip) {
    clip.frameCount = std::max(clip.frameCount, 1);
    clip.frameTime = std::max(clip.frameTime, 0.001f);
    clips_[name] = clip;

    if (currentClip_.empty()) {
        play(name);
    }
}

void AnimationPlayer::play(const std::string& name) {
    if (currentClip_ == name || !clips_.contains(name)) {
        return;
    }

    currentClip_ = name;
    currentFrame_ = 0;
    elapsed_ = 0.0f;
}

void AnimationPlayer::update(float dt) {
    if (currentClip_.empty()) {
        return;
    }

    const AnimationClip& clip = clips_.at(currentClip_);
    elapsed_ += dt;

    while (elapsed_ >= clip.frameTime) {
        elapsed_ -= clip.frameTime;
        ++currentFrame_;

        if (currentFrame_ >= clip.frameCount) {
            currentFrame_ = clip.loop ? 0 : clip.frameCount - 1;
        }
    }
}

void AnimationPlayer::draw(Vector2 position, float scale, bool flipX, Color tint) const {
    if (!texture_ || currentClip_.empty()) {
        return;
    }

    Rectangle source = sourceRectangle();
    if (flipX) {
        source.width *= -1.0f;
    }

    const Rectangle dest{
        position.x,
        position.y,
        std::abs(source.width) * scale,
        source.height * scale
    };

    DrawTexturePro(*texture_, source, dest, Vector2{}, 0.0f, tint);
}

Rectangle AnimationPlayer::sourceRectangle() const {
    const AnimationClip& clip = clips_.at(currentClip_);
    const int frame = clip.firstFrame + currentFrame_;
    const int columns = std::max(texture_->width / frameWidth_, 1);
    const int x = frame % columns;
    const int y = frame / columns;

    return Rectangle{
        static_cast<float>(x * frameWidth_),
        static_cast<float>(y * frameHeight_),
        static_cast<float>(frameWidth_),
        static_cast<float>(frameHeight_)
    };
}

} // namespace game
