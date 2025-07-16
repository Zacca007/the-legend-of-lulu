#include "animationHandler.hpp"
#include <cstring>

namespace lulu
{

AnimationHandler::AnimationHandler()
    : _currentDirection(D_STILL), _currentFrame(0), _currentState(S_STILL), _enabled(false)
{
}

AnimationHandler::AnimationHandler(const bool enableAnimation, const direction initialDirection,
                                   const state initialState)
    : _currentDirection(initialDirection), _currentFrame(0), _currentState(initialState), _enabled(enableAnimation)
{
    // Reset to default state if animation is disabled
    if (!enableAnimation)
    {
        _currentState = S_STILL;
        _currentDirection = D_STILL;
    }
}

// === Getters ===
direction AnimationHandler::currentDirection() const
{
    return _currentDirection;
}

std::uint8_t AnimationHandler::currentFrame() const
{
    return _currentFrame;
}

state AnimationHandler::currentState() const
{
    return _currentState;
}

// === Internal helpers ===
std::uint32_t AnimationHandler::readBigEndian(std::ifstream &file)
{
    uint8_t bytes[4];
    file.read(reinterpret_cast<char *>(bytes), 4);
    // Convert from big-endian to host byte order
    return static_cast<uint32_t>(bytes[0]) << 24 | static_cast<uint32_t>(bytes[1]) << 16 |
           static_cast<uint32_t>(bytes[2]) << 8 | static_cast<uint32_t>(bytes[3]);
}

// === Animation control ===
const std::vector<std::string> &AnimationHandler::currentAnimation() const
{
    return _animationSet.at(_currentState).at(_currentDirection);
}

void AnimationHandler::addAnimation(const state state, const direction direction,
                                    const std::vector<std::string> &animation)
{
    _animationSet[state][direction] = animation;
}

void AnimationHandler::set(const state newState, const direction newDirection)
{
    _currentState = newState;
    _currentDirection = newDirection;
    _currentFrame = 0;
}

std::string AnimationHandler::nextSprite()
{
    // Validate animation data exists
    if (_animationSet.empty() || _animationSet[_currentState].empty() ||
        _animationSet[_currentState][_currentDirection].empty())
    {
        return "";
    }

    // Get current sprite and advance frame counter
    std::string sprite = _animationSet[_currentState][_currentDirection][_currentFrame];
    _currentFrame = (_currentFrame + 1) % _animationSet[_currentState][_currentDirection].size();
    return sprite;
}

std::optional<pair> AnimationHandler::getSpriteDimension(const std::string &filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file)
        return std::nullopt;

    // PNG file signature (8 bytes)
    constexpr uint8_t expected_signature[8] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
    uint8_t signature[8];
    file.read(reinterpret_cast<char *>(signature), 8);

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

    return pair{static_cast<float>(width), static_cast<float>(height)};
}

} // namespace lulu
