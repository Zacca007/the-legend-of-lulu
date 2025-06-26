#include <raylib.h>
#include "lulu.hpp"
int main(void)
{
    const lulu::pair screenSize{800, 550}, arenaSize{600, 350}, arenaPos{100, 100};
    lulu::Arena arena(arenaPos, arenaSize);
    InitWindow(screenSize.x, screenSize.y, "legend of lulu");
    Texture2D bg = LoadTexture("core/assets/rooms/room1.png");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        DrawTexture(bg, 0, 0, WHITE);
        DrawRectangleLines(arena.pos().x, arena.pos().y, arena.size().x, arena.size().y, YELLOW);
        for (int i = 1; i < 5; i++)
        {
            lulu::Actor act(arenaPos + (50 * i), {50, 50});
            arena.spawn(&act);
            DrawRectangleLines(act.pos().x, act.pos().y, act.size().x, act.size().y, YELLOW);
        }
        EndDrawing();
    }
    for (lulu::Actor *act : arena.actors())
    {
        arena.kill(act);
    }
    CloseWindow();

    return 0;
}