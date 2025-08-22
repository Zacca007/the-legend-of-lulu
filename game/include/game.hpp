#pragma once
#include "gameScene.hpp"
#include <memory>

namespace game
{
class Game final
{
    // TODO: replace with lulu::pair
    const int SCREEN_X = 800, SCREEN_Y = 550;
    std::unique_ptr<GameScene> scene_;

  public:
    Game();
    ~Game();

    void run() const;
};
}