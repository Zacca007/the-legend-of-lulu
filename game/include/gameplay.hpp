#pragma once
#include "gameScene.hpp"

namespace game
{
class Gameplay final : public GameScene {

public:
    explicit Gameplay(Game *game,
            const std::string &background = "assets/dungeon/dungeon hall.png",
            const std::string &music = "assets/dungeon/dungeon.mp3",
            const std::vector<lulu::Key> &inputKeys = std::vector{lulu::K_A});


    void tick() override;
    void render() override;
};
}