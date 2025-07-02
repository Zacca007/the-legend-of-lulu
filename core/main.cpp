
#include <raylib.h>

#include <array>
#include <iostream>
#include <vector>

#include "lulu.hpp"

class Game
{
    static constexpr lulu::pair SCREEN_SIZE{800, 550};
    static constexpr lulu::pair ARENA_SIZE{600, 350};
    static constexpr lulu::pair ARENA_POS{100, 100};
    static constexpr int CELL_SIZE = 50;

    static constexpr std::array<lulu::Key, 4> INPUT_KEYS = {lulu::K_W, lulu::K_A, lulu::K_S, lulu::K_D};

    // Posizioni degli attori statici in coordinate griglia
    static constexpr std::array<lulu::pair, 12> STATIC_ACTOR_POSITIONS = {
        {{1, 1}, {1, 3}, {1, 5}, {4, 1}, {4, 3}, {4, 5}, {7, 1}, {7, 3}, {7, 5}, {10, 1}, {10, 3}, {10, 5}}};

    lulu::Arena arena;
    std::vector<lulu::Actor> staticActors;
    lulu::Link link;
    Texture2D background;

  public:
    Game()
        : arena(ARENA_POS, ARENA_SIZE), link(ARENA_POS + (ARENA_SIZE / 2), {30, 30}, 5, 12, 1, &arena)

    {
        initializeStaticActors();
        initializeGraphics();
    }

    ~Game()
    {
        cleanup();
    }

    void run()
    {
        while (!WindowShouldClose())
        {
            update();
            render();
            std::cout << "link è in ccollisione con gli attori in queste posizioni:" << std::endl;
            auto pair = arena.collisions().find(&link);
            for (const auto &collision : pair->second)
            {
                std::cout << "x: " << collision.target->pos().x << ", y: " << collision.target->pos().y << std::endl;
            }
            std::cout << std::endl;
        }
    }

  private:
    void initializeStaticActors()
    {
        staticActors.reserve(STATIC_ACTOR_POSITIONS.size());

        for (const auto &gridPos : STATIC_ACTOR_POSITIONS)
        {
            lulu::pair worldPos = ARENA_POS + lulu::pair{gridPos.x * CELL_SIZE, gridPos.y * CELL_SIZE};
            staticActors.emplace_back(worldPos, lulu::pair{CELL_SIZE, CELL_SIZE}, &arena);
        }
    }

    void initializeGraphics()
    {
        InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "Legend of Lulu");
        background = LoadTexture("core/assets/rooms/room1.png");
        SetTargetFPS(60);
    }

    std::vector<lulu::Key> getActiveKeys() const
    {
        std::vector<lulu::Key> activeKeys;
        for (lulu::Key key : INPUT_KEYS)
            if (IsKeyDown(key))
                activeKeys.push_back(key);
        return activeKeys;
    }

    void update()
    {
        std::vector<lulu::Key> activeKeys = getActiveKeys();
        arena.tick(activeKeys);
    }

    void render()
    {
        BeginDrawing();

        DrawTexture(background, 0, 0, WHITE);
        drawArena();
        drawStaticActors();
        drawLink();

        EndDrawing();
    }

    void drawArena() const
    {
        DrawRectangleLines(arena.pos().x, arena.pos().y, arena.size().x, arena.size().y, YELLOW);
    }

    void drawStaticActors() const
    {
        for (const auto &actor : staticActors)
            DrawRectangleLines(actor.pos().x, actor.pos().y, actor.size().x, actor.size().y, YELLOW);
    }

    void drawLink() const
    {
        DrawRectangle(link.pos().x, link.pos().y, link.size().x, link.size().y, YELLOW);
    }

    void cleanup()
    {
        for (lulu::Actor *actor : arena.actors())
            arena.kill(actor);
        UnloadTexture(background);
        CloseWindow();
    }
};

int main()
{
    Game game;
    game.run();
    return 0;
}
