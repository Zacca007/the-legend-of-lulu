#pragma once
#include "gameScene.hpp"

namespace game
{
class Gameplay final : public GameScene
{
    lulu::Arena arena_;

    // TODO: define method to parse arena and then call it in this constructor
    static lulu::Arena parserArena(const std::string &configPath);
  public:
    explicit Gameplay(Game *game, const std::string &configPath = "assets/dungeon/configs/hall.json");

    void tick() override;
    void render() override;
};
} // namespace game