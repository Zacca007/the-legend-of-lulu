#pragma once
#include "lulu.hpp"
#include <memory>

namespace game
{
class GameScene;
class Game final
{
  private:
    const lulu::Vec2<int> SCREEN_SIZE{800, 550};
    std::unique_ptr<GameScene> scene_;

  public:
    Game();
    ~Game();

    void run() const;
    void switchScene(std::unique_ptr<GameScene> &newScene);
};
}