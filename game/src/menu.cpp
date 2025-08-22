#include "menu.hpp"

namespace game
{
Menu::Menu(const std::string &background, const std::string &music, const std::vector<int> &inputs)
    : GameScene(background, music, inputs), transparency(0), growing(true)
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
}

void Menu::render()
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(background_, 0, 0, WHITE);
    DrawText("Press SPACE to start", 225, 500, 30, {255, 255, 255, transparency});
    EndDrawing();
}
} // namespace game