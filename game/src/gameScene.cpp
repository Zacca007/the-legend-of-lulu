#include "gameScene.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

namespace game
{
GameScene::GameScene(Game *game, const std::string &background, const std::string &music, const std::vector<lulu::Key> &inputs)
    : inputs_(inputs), game_(game)
{
    background_ = LoadTexture(background.c_str());
    music_ = LoadMusicStream(music.c_str());
    PlayMusicStream(music_);
}

GameScene::GameScene(Game *game, const std::string &configPath)
    : game_(game)
{
    std::ifstream f(configPath);
    if (!f.is_open())
    {
        throw std::runtime_error("Could not open config file: " + configPath);
    }

    nlohmann::json j;
    f >> j;

    // leggi i campi
    auto background = j.at("background").get<std::string>();
    auto music = j.at("music").get<std::string>();

    // array -> vector<lulu::Key>
    inputs_.clear();
    for (const auto &val : j.at("inputs")) {
        inputs_.push_back(static_cast<lulu::Key>(val.get<int>()));
    }

    // inizializza Raylib
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