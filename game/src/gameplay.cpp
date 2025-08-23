#include "gameplay.hpp"

namespace game
{
Gameplay::Gameplay(Game *game, const std::string &background, const std::string &music,
                   const std::vector<lulu::Key> &inputKeys)
    : GameScene(game, background, music, inputKeys)
{
}

void Gameplay::tick()
{
    UpdateMusicStream(music_);
}

void Gameplay::render()
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(background_, 0, 0, WHITE);

    EndDrawing();
}
}