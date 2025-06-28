#include <raylib.h>
#include "lulu.hpp"
#include <iostream>
#include <vector>
#include <array>

std::vector<lulu::Key> getCurrentKeys()
{
    std::vector<lulu::Key> active_keys;
    static const std::array<lulu::Key, 4> keys_to_check = {lulu::K_DOWN, lulu::K_LEFT, lulu::K_RIGHT, lulu::K_UP};
    
    for (lulu::Key key : keys_to_check)
    {
        if (IsKeyDown(key))
        {
            active_keys.push_back(key);
        }
    }
    return active_keys;
}

int main(void)
{
    const lulu::pair screenSize{800, 550}, arenaSize{600, 350}, arenaPos{100, 100};
    lulu::Arena arena(arenaPos, arenaSize);

    // Attori statici alle coordinate specificate (coordinate * 50 + arenaPos)
    lulu::Actor act1(arenaPos + lulu::pair{1 * 50, 1 * 50}, {50, 50}, &arena);
    lulu::Actor act2(arenaPos + lulu::pair{1 * 50, 3 * 50}, {50, 50}, &arena);
    lulu::Actor act3(arenaPos + lulu::pair{1 * 50, 5 * 50}, {50, 50}, &arena);
    lulu::Actor act4(arenaPos + lulu::pair{4 * 50, 1 * 50}, {50, 50}, &arena);
    lulu::Actor act5(arenaPos + lulu::pair{4 * 50, 3 * 50}, {50, 50}, &arena);
    lulu::Actor act6(arenaPos + lulu::pair{4 * 50, 5 * 50}, {50, 50}, &arena);
    lulu::Actor act7(arenaPos + lulu::pair{7 * 50, 1 * 50}, {50, 50}, &arena);
    lulu::Actor act8(arenaPos + lulu::pair{7 * 50, 3 * 50}, {50, 50}, &arena);
    lulu::Actor act9(arenaPos + lulu::pair{7 * 50, 5 * 50}, {50, 50}, &arena);
    lulu::Actor act10(arenaPos + lulu::pair{9 * 50, 1 * 50}, {50, 50}, &arena);
    lulu::Actor act11(arenaPos + lulu::pair{9 * 50, 3 * 50}, {50, 50}, &arena);
    lulu::Actor act12(arenaPos + lulu::pair{9 * 50, 5 * 50}, {50, 50}, &arena);

    // Link che si può muovere
    lulu::Link link(arenaPos + lulu::pair{arenaSize.x / 2, arenaSize.y / 2}, {50, 50}, {5, 5}, &arena);

    InitWindow(screenSize.x, screenSize.y, "legend of lulu");
    Texture2D bg = LoadTexture("core/assets/rooms/room1.png");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Ottieni i tasti premuti
        std::vector<lulu::Key> active_keys = getCurrentKeys();

        // Aggiorna l'arena (questo farà muovere Link)
        arena.tick(active_keys);

        BeginDrawing();
        DrawTexture(bg, 0, 0, WHITE);

        // Disegna il bordo dell'arena
        DrawRectangleLines(arena.pos().x, arena.pos().y, arena.size().x, arena.size().y, YELLOW);

        // Disegna tutti gli attori statici
        DrawRectangleLines(act1.pos().x, act1.pos().y, act1.size().x, act1.size().y, YELLOW);
        DrawRectangleLines(act2.pos().x, act2.pos().y, act2.size().x, act2.size().y, YELLOW);
        DrawRectangleLines(act3.pos().x, act3.pos().y, act3.size().x, act3.size().y, YELLOW);
        DrawRectangleLines(act4.pos().x, act4.pos().y, act4.size().x, act4.size().y, YELLOW);
        DrawRectangleLines(act5.pos().x, act5.pos().y, act5.size().x, act5.size().y, YELLOW);
        DrawRectangleLines(act6.pos().x, act6.pos().y, act6.size().x, act6.size().y, YELLOW);
        DrawRectangleLines(act7.pos().x, act7.pos().y, act7.size().x, act7.size().y, YELLOW);
        DrawRectangleLines(act8.pos().x, act8.pos().y, act8.size().x, act8.size().y, YELLOW);
        DrawRectangleLines(act9.pos().x, act9.pos().y, act9.size().x, act9.size().y, YELLOW);
        DrawRectangleLines(act10.pos().x, act10.pos().y, act10.size().x, act10.size().y, YELLOW);
        DrawRectangleLines(act11.pos().x, act11.pos().y, act11.size().x, act11.size().y, YELLOW);
        DrawRectangleLines(act12.pos().x, act12.pos().y, act12.size().x, act12.size().y, YELLOW);

        // Disegna Link con un colore diverso per distinguerlo
        DrawRectangle(link.pos().x, link.pos().y, link.size().x, link.size().y, YELLOW);

        EndDrawing();
    }

    std::cout << "il numero di attori è: " << arena.actors().size() << std::endl;

    for (lulu::Actor *act : arena.actors())
    {
        arena.kill(act);
    }

    UnloadTexture(bg);
    CloseWindow();
    return 0;
}