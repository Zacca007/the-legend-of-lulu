#include <raylib.h>

#include <array>
#include <iostream>
#include <utility>
#include <vector>

#include "lulu.hpp"

class Game
{
    static constexpr lulu::pair SCREEN_SIZE{800, 550};
    static constexpr lulu::pair ARENA_SIZE{600, 350};
    static constexpr lulu::pair ARENA_POS{100, 100};
    static constexpr int CELL_SIZE = 50;

    static constexpr std::array<lulu::Key, 4> INPUT_KEYS = {lulu::K_W, lulu::K_A, lulu::K_S, lulu::K_D};

    static constexpr std::array<lulu::pair, 12> STATIC_ACTOR_POSITIONS = {
        {{1, 1}, {1, 3}, {1, 5}, {4, 1}, {4, 3}, {4, 5}, {7, 1}, {7, 3}, {7, 5}, {10, 1}, {10, 3}, {10, 5}}};

    lulu::Arena arena;
    std::vector<lulu::Actor> staticActors;
    lulu::Link link;
    Texture2D background;
    Texture2D linkSprite;

  public:
    Game() : arena(ARENA_POS, ARENA_SIZE), link(ARENA_POS + (ARENA_SIZE / 2), {50, 50}, 10, 12, 1, &arena)
    {
        initializeStaticActors();
        initializeGraphics();
    }

    ~Game()
    {
        for (auto *actor : arena.actors())
            arena.kill(actor);
        UnloadTexture(background);
        CloseWindow();
    }

    void run()
    {
        while (!WindowShouldClose())
        {
            update();
            render();
        }
    }

  private:
    void initializeStaticActors()
    {
        staticActors.reserve(STATIC_ACTOR_POSITIONS.size());

        for (const auto &gridPos : STATIC_ACTOR_POSITIONS)
        {
            const lulu::pair worldPos = ARENA_POS + lulu::pair{gridPos.x * CELL_SIZE, gridPos.y * CELL_SIZE};
            staticActors.emplace_back(worldPos, lulu::pair{CELL_SIZE, CELL_SIZE}, &arena);
        }
    }

    void initializeGraphics()
    {
        InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "Legend of Lulu");
        background = LoadTexture("core/assets/rooms/dungeon hall.png");
        linkSprite = LoadTexture(link.sprite().data());
        SetTargetFPS(10);
    }

    [[nodiscard]]
    std::vector<lulu::Key> getActiveKeys() const
    {
        std::vector<lulu::Key> keys;
        keys.reserve(INPUT_KEYS.size());
        for (lulu::Key key : INPUT_KEYS)
            if (IsKeyDown(key))
                keys.push_back(key);
        return keys;
    }

    void update()
    {
        arena.tick(getActiveKeys());
        linkSprite = LoadTexture(link.sprite().data());
    }

    void render()
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background, 0, 0, WHITE);

        drawRectOutline(arena.pos(), arena.size(), YELLOW);

        for (const auto &actor : staticActors)
            drawRectOutline(actor.pos(), actor.size(), YELLOW);

        DrawTexture(linkSprite, static_cast<int>(link.pos().x), static_cast<int>(link.pos().y), WHITE);

        EndDrawing();
    }

    static void drawRectOutline(const lulu::pair &pos, const lulu::pair &size, Color color)
    {
        DrawRectangleLines(pos.x, pos.y, size.x, size.y, color);
    }

    static void drawRectFilled(const lulu::pair &pos, const lulu::pair &size, Color color)
    {
        DrawRectangle(pos.x, pos.y, size.x, size.y, color);
    }
};

int main()
{
    Game game;
    game.run();
    return 0;
}
