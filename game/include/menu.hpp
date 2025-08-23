#pragma once
#include "gameScene.hpp"

namespace game
{
class Menu final : public GameScene
{
  private:
    unsigned char transparency;
    bool growing;

  public:
    explicit Menu(Game *game, const std::string &background = "assets/menu/menu.png",
                  const std::string &music = "assets/menu/menu.mp3",
                  const std::vector<lulu::Key> &inputs = std::vector{lulu::K_ENTER});

    void tick() override;
    void render() override;
};
}
