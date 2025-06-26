#include <raylib.h>
#include "lulu.hpp"
int main(void)
{
    const lulu::pair screenSize{800, 550}, arenaSize{600, 350}, arenaPos{100, 100};
    lulu::Arena arena(arenaPos, arenaSize);
    lulu::Actor block(arenaPos+50, {50, 50}, &arena);
    InitWindow(screenSize.x, screenSize.y, "lulù ti amo");
    Texture2D bg = LoadTexture("core/assets/rooms/room1.png");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        DrawTexture(bg, 0, 0, WHITE);
        DrawRectangleLines(arena.pos().x, arena.pos().y, arena.size().x, arena.size().y, YELLOW);
        DrawRectangleLines(block.pos().x, block.pos().y, block.size().x, block.size().y, YELLOW);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}