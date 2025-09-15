#include "game.hpp"
#include "menu.hpp"

namespace game
{
Game::Game()
{
    InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "The Legend of LuLù");
    SetTargetFPS(30);
    InitAudioDevice();

    scene_ = std::make_unique<Menu>(this);
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

void Game::switchScene(std::unique_ptr<GameScene> &newScene)
{
    scene_ = std::move(newScene);
}
} // namespace game