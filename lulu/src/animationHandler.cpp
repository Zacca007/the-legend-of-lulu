#include "animationHandler.hpp"
#include <cstring>

namespace lulu
{
    AnimationHandler::AnimationHandler() : movementDirection_(D_NONE), frame_(0), state_(S_STILL), enabled_(false)
    {
    }

    AnimationHandler::AnimationHandler(const bool enableAnimation, const Direction initialDirection,
                                       const State initialState)
        : movementDirection_(initialDirection), frame_(0), state_(initialState), enabled_(enableAnimation)
    {
        // Reset to default state if animation is disabled
        if (!enableAnimation)
        {
            state_ = S_STILL;
            movementDirection_ = D_NONE;
        }
    }

    // === Getters ===
    Direction AnimationHandler::currentDirection() const
    {
        return movementDirection_;
    }

    std::uint8_t AnimationHandler::currentFrame() const
    {
        return frame_;
    }

    State AnimationHandler::currentState() const
    {
        return state_;
    }

    // === Internal helpers ===
    std::uint32_t AnimationHandler::readBigEndian(std::ifstream& file)
    {
        uint8_t bytes[4];
        file.read(reinterpret_cast<char*>(bytes), 4);
        // Convert from big-endian to host byte order
        return static_cast<uint32_t>(bytes[0]) << 24 | static_cast<uint32_t>(bytes[1]) << 16 |
            static_cast<uint32_t>(bytes[2]) << 8 | static_cast<uint32_t>(bytes[3]);
    }

    // === Animation control ===
    const std::vector<std::string>& AnimationHandler::currentAnimation() const
    {
        return animationSet_.at(state_).at(movementDirection_);
    }

    void AnimationHandler::addAnimation(const State state, const Direction direction,
                                        const std::vector<std::string>& animation)
    {
        animationSet_[state][direction] = animation;
    }

    void AnimationHandler::set(const State newState, const Direction newDirection)
    {
        state_ = newState;
        movementDirection_ = newDirection;
        frame_ = 0;
    }

    std::string AnimationHandler::nextSprite()
    {
        // Validate animation data exists
        if (animationSet_.empty() || animationSet_[state_].empty() || animationSet_[state_][movementDirection_].empty())
        {
            return "";
        }

        // Get current sprite and advance frame counter
        std::string sprite = animationSet_[state_][movementDirection_][frame_];
        frame_ = (frame_ + 1) % animationSet_[state_][movementDirection_].size();
        return sprite;
    }

    std::optional<Vec2<float>> AnimationHandler::getSpriteDimension(const std::string& filepath)
    {
        std::ifstream file(filepath, std::ios::binary);
        if (!file)
            return std::nullopt;

        // PNG file signature (8 bytes)
        constexpr uint8_t expected_signature[8] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
        uint8_t signature[8];
        file.read(reinterpret_cast<char*>(signature), 8);

        // Verify PNG signature
        if (!file || std::memcmp(signature, expected_signature, 8) != 0)
            return std::nullopt;

        file.ignore(4); // Skip chunk length (4 bytes)

        // Read chunk type (should be "IHDR" for header)
        char chunkType[5] = {};
        file.read(chunkType, 4);
        if (std::string(chunkType) != "IHDR")
            return std::nullopt;

        // Read width and height from IHDR chunk
        const uint32_t width = readBigEndian(file);
        const uint32_t height = readBigEndian(file);

        return Vec2(width, height).convert<float>();
    }
} // namespace lulu
