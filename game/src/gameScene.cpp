#include "gameScene.hpp"

namespace game
{
GameScene::GameScene(Game *game, const std::string &background, const std::string &music, const std::vector<lulu::Key> &inputs)
    : inputs_(inputs), game_(game)
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

std::vector<lulu::Key> GameScene::activeInputs() const
{
    std::vector<lulu::Key> keys;
    keys.reserve(inputs_.size());
    for (lulu::Key key : inputs_)
        if (IsKeyDown(key))
            keys.push_back(key);
    return keys;
}
}