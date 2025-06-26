#include <raylib.h>
#include "lulu.hpp"
#include <iostream>

int main(void)
{
    const lulu::pair screenSize{800, 550}, arenaSize{600, 350}, arenaPos{100, 100};
    lulu::Arena arena(arenaPos, arenaSize);
    lulu::Actor act1(arenaPos + 50 * 1, {50, 50}, &arena);
    lulu::Actor act2(arenaPos + 50 * 2, {50, 50}, &arena);
    lulu::Actor act3(arenaPos + 50 * 3, {50, 50}, &arena);

    InitWindow(screenSize.x, screenSize.y, "legend of lulu");
    Texture2D bg = LoadTexture("core/assets/rooms/room1.png");

    lulu::Actor act4(arenaPos + 50 * 4, {50, 50});
    lulu::Actor act5(arenaPos + 50 * 5, {50, 50});
    lulu::Actor act6(arenaPos + 50 * 6, {50, 50});
    arena.spawn(&act4);
    arena.spawn(&act5);
    arena.spawn(&act6);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        DrawTexture(bg, 0, 0, WHITE);
        DrawRectangleLines(arena.pos().x, arena.pos().y, arena.size().x, arena.size().y, YELLOW);

        DrawRectangleLines(act1.pos().x, act1.pos().y, act1.size().x, act1.size().y, YELLOW);
        DrawRectangleLines(act2.pos().x, act2.pos().y, act2.size().x, act2.size().y, YELLOW);
        DrawRectangleLines(act3.pos().x, act3.pos().y, act3.size().x, act3.size().y, YELLOW);
        DrawRectangleLines(act4.pos().x, act4.pos().y, act4.size().x, act4.size().y, YELLOW);
        DrawRectangleLines(act5.pos().x, act5.pos().y, act5.size().x, act5.size().y, YELLOW);
        DrawRectangleLines(act6.pos().x, act6.pos().y, act6.size().x, act6.size().y, YELLOW);

        EndDrawing();
    }
    std::cout << "il numero di attori è: " << arena.actors().size() << std::endl;
    for (lulu::Actor *act : arena.actors())
    {
        arena.kill(act);
    }
    CloseWindow();

    return 0;
}