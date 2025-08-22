#include "gameScene.hpp"

namespace game
{
GameScene::GameScene(const std::string &background, const std::string &music, const std::vector<int> &inputs)
    : inputs_(inputs)
{
    background_ = LoadTexture(background.c_str());
    music_ = LoadMusicStream(music.c_str());
    PlayMusicStream(music_);
}

GameScene::~GameScene()
{
    UnloadTexture(background_);
    UnloadMusicStream(music_);
}

std::vector<int> GameScene::activeInputs() const
{
    // TODO: replace int with lulu::key
    std::vector<int> keys;
    keys.reserve(inputs_.size());
    for (int key : inputs_)
        if (IsKeyDown(key))
            keys.push_back(key);
    return keys;
}
} // namespace game