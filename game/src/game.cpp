#include "game.hpp"
#include "menu.hpp"

namespace game
{
Game::Game()
{
    InitWindow(SCREEN_X, SCREEN_Y, "The Legend of LuLù");
    SetTargetFPS(30);
    InitAudioDevice();

    scene_ = std::make_unique<Menu>();
}

Game::~Game()
{
    CloseAudioDevice();
    CloseWindow();
}

void Game::run() const
{
    while (!WindowShouldClose())
    {
        scene_->tick();
        scene_->render();
    }
}
}