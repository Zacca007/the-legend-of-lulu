#include "gameScene.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

namespace game
{
GameScene::GameScene(Game *game, const std::string &background, const std::string &music,
                     const std::vector<lulu::Key> &inputs)
    : inputs_(inputs), game_(game)
{
    background_ = LoadTexture(background.c_str());
    music_ = LoadMusicStream(music.c_str());
    PlayMusicStream(music_);
}

GameScene::GameScene(Game *game, const std::string &configPath) : game_(game)
{
    setBackground(configPath);
    setMusic(configPath);
    setInputs(configPath);
}

GameScene::~GameScene()
{
    UnloadTexture(background_);
    UnloadMusicStream(music_);
}

void GameScene::setBackground(const std::string &configPath)
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

    if (background_.id != 0)
    {
        UnloadTexture(background_);
    }
    background_ = LoadTexture(background.c_str());
}

void GameScene::setMusic(const std::string &configPath)
{
    std::ifstream f(configPath);
    if (!f.is_open())
    {
        throw std::runtime_error("Could not open config file: " + configPath);
    }

    nlohmann::json j;
    f >> j;

    auto music = j.at("music").get<std::string>();
    if (music_.ctxData != nullptr) {
        UnloadMusicStream(music_);
    }
    music_ = LoadMusicStream(music.c_str());
    PlayMusicStream(music_);
}

void GameScene::setInputs(const std::string &configPath)
{
    std::ifstream f(configPath);
    if (!f.is_open())
    {
        throw std::runtime_error("Could not open config file: " + configPath);
    }

    nlohmann::json j;
    f >> j;

    inputs_.clear();
    for (const auto &val : j.at("inputs"))
    {
        inputs_.push_back(static_cast<lulu::Key>(val.get<int>()));
    }
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
} // namespace game