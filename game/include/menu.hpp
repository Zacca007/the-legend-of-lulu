#pragma once
#include "gameScene.hpp"

namespace game
{
class Menu final : public GameScene
{
    unsigned char transparency;
    bool growing;

  public:
    // TODO: replace int with lulu::key
    explicit Menu(const std::string &background = "assets/menu/menu.png",
                  const std::string &music = "assets/menu/menu.mp3",
                  const std::vector<int> &inputs = std::vector{32});

    void tick() override;
    void render() override;
};
} // namespace game
