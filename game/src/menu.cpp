#include "menu.hpp"
#include "game.hpp"
#include "gameplay.hpp"

#include <memory>

namespace game
{
    Menu::Menu(Game* game, const std::string& background, const std::string& music,
               const std::vector<lulu::Key>& inputs)
        : GameScene(game, background, music, inputs), transparency(0), growing(true)
    {
        music_.looping = false;
    }

    void Menu::tick()
    {
        UpdateMusicStream(music_);
        if (transparency == 255)
            growing = false;
        else if (transparency == 0)
            growing = true;
        transparency += growing ? 15 : -15;

        for (const auto keys = activeInputs(); const auto key : keys)
        {
            if (key == lulu::K_ENTER)
            {
                std::unique_ptr<GameScene> gameplay_scene = std::make_unique<Gameplay>(game_);
                game_->switchScene(gameplay_scene);
                break;
            }
        }
    }

    void Menu::render()
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background_, 0, 0, WHITE);
        DrawText("Press ENTER to start", 225, 500, 30, {255, 255, 255, transparency});
        EndDrawing();
    }
} // namespace game
