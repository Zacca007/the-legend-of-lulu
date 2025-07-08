#include "lulu.hpp"
#include <array>
#include <iostream>
#include <raylib.h>
#include <unordered_map>
#include <vector>

class Game
{
  private:
    static constexpr lulu::pair SCREEN_SIZE{800, 550};
    static constexpr lulu::pair ARENA_SIZE{600, 350};
    static constexpr lulu::pair ARENA_POS{100, 100};
    static constexpr int CELL_SIZE = 50;
    static constexpr std::array<lulu::Key, 9> INPUT_KEYS = {lulu::K_W,     lulu::K_A,    lulu::K_S,
                                                            lulu::K_D,     lulu::K_DOWN, lulu::K_LEFT,
                                                            lulu::K_RIGHT, lulu::K_UP,   lulu::K_SPACE};
    static constexpr std::array<lulu::pair, 12> STATIC_ACTOR_POSITIONS = {
        {{1, 1}, {1, 3}, {1, 5}, {4, 1}, {4, 3}, {4, 5}, {7, 1}, {7, 3}, {7, 5}, {10, 1}, {10, 3}, {10, 5}}};

    lulu::Arena arena;
    std::vector<lulu::Actor> staticActors;
    lulu::Link link;

    Texture2D background;
    std::unordered_map<std::string, Texture2D> textureCache;

  public:
    Game() : arena(ARENA_POS, ARENA_SIZE), link(ARENA_POS + (ARENA_SIZE / 2), {50, 50}, 10, 12, 1, &arena)
    {
        initializeGraphics();
        initializeStaticActors();
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
        }
    }

  private:
    void initializeGraphics()
    {
        InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "Legend of Lulu");
        background = LoadTexture("core/assets/rooms/dungeon hall.png");
        SetTargetFPS(10);
    }

    void initializeStaticActors()
    {
        staticActors.reserve(STATIC_ACTOR_POSITIONS.size());
        for (const auto &gridPos : STATIC_ACTOR_POSITIONS)
        {
            const lulu::pair worldPos = ARENA_POS + lulu::pair{gridPos.x * CELL_SIZE, gridPos.y * CELL_SIZE};
            staticActors.emplace_back(worldPos, lulu::pair{CELL_SIZE, CELL_SIZE}, &arena);
        }
    }

    void cleanup()
    {
        for (auto *actor : arena.actors())
            arena.kill(actor);

        for (auto &[path, texture] : textureCache)
            UnloadTexture(texture);

        UnloadTexture(background);
        CloseWindow();
    }

    Texture2D getTexture(const std::string &path)
    {
        auto it = textureCache.find(path);
        if (it == textureCache.end())
        {
            textureCache[path] = LoadTexture(path.c_str());
            return textureCache[path];
        }
        return it->second;
    }

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
    }

    void render()
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexture(background, 0, 0, WHITE);

        Texture2D linkTexture = getTexture(link.sprite());
        DrawTexture(linkTexture, static_cast<int>(link.pos().x), static_cast<int>(link.pos().y), WHITE);

        EndDrawing();
    }

    void drawRectOutline(const lulu::pair &pos, const lulu::pair &size, Color color)
    {
        DrawRectangleLines(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x),
                           static_cast<int>(size.y), color);
    }
};

int main()
{
    Game game;
    game.run();
    return 0;
}