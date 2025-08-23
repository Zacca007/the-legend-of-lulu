#pragma once
#include "lulu.hpp"
#include <raylib.h>
#include <string>
#include <vector>

namespace game
{
class Game;
class GameScene
{
  protected:
    Texture2D background_{};
    Music music_{};
    std::vector<lulu::Key> inputs_;
    Game *game_;

    [[nodiscard]] std::vector<lulu::Key> activeInputs() const;

  public:
    GameScene(Game *game, const std::string &background, const std::string &music, const std::vector<lulu::Key> &inputs);
    GameScene(Game *game, const std::string &configPath);
    virtual ~GameScene();

    virtual void tick() = 0;
    virtual void render() = 0;
};
}
