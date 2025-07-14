#include "lulu.hpp"
#include <array>
#include <memory>
#include <ranges>
#include <raylib.h>
#include <unordered_map>
#include <vector>

// Forward declaration
class Game;

class GameScene
{
  protected:
    Texture2D _background{};
    Music _music{};
    std::map<std::string, Sound> _sounds;
    std::vector<lulu::Key> _inputKeys;
    Game *_game; // Riferimento al game per poter cambiare scena

  public:
    GameScene(const std::string &background, const std::string &music, const std::vector<std::string> &sounds,
              const std::vector<lulu::Key> &inputKeys, Game *game = nullptr)
        : _inputKeys(inputKeys), _game(game)
    {
        _background = LoadTexture(background.c_str());
        _music = LoadMusicStream(music.c_str());
        for (const auto &sound : sounds)
        {
            _sounds[sound] = LoadSound(sound.c_str());
        }
    }

    virtual ~GameScene()
    {
        UnloadTexture(_background);
        UnloadMusicStream(_music);
        for (const auto &sound : _sounds | std::views::values)
            UnloadSound(sound);
    }

    virtual void tick() = 0;
    virtual void render() = 0;

  protected:
    [[nodiscard]] std::vector<lulu::Key> getActiveKeys() const
    {
        std::vector<lulu::Key> keys;
        keys.reserve(_inputKeys.size());
        for (lulu::Key key : _inputKeys)
            if (IsKeyDown(key))
                keys.push_back(key);
        return keys;
    }
};

class Menu final : public GameScene
{
    unsigned char transparency = 0;
    bool growing = true;

  public:
    Menu(const std::string &background, const std::string &music, const std::vector<std::string> &sounds,
         const std::vector<lulu::Key> &inputKeys, Game *game)
        : GameScene(background, music, sounds, inputKeys, game)
    {
        _music.looping = false;
        PlayMusicStream(_music);
    }

    void tick() override;
    void render() override
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(_background, 0, 0, WHITE);
        DrawText("Press SPACE to start", 225, 500, 30, {255, 255, 255, transparency});
        EndDrawing();
        if (transparency == 255)
            growing = false;
        else if (transparency == 0)
            growing = true;
        transparency += growing ? 15 : -15;
    }
};

class GameOn final : public GameScene
{
  private:
    static constexpr lulu::pair ARENA_SIZE{600, 350};
    static constexpr lulu::pair ARENA_POS{100, 100};
    static constexpr int CELL_SIZE = 50;
    static constexpr std::array<lulu::pair, 12> STATIC_ACTOR_POSITIONS = {
        {{1, 1}, {1, 3}, {1, 5}, {4, 1}, {4, 3}, {4, 5}, {7, 1}, {7, 3}, {7, 5}, {10, 1}, {10, 3}, {10, 5}}};

    lulu::Arena arena;
    std::vector<lulu::Actor> staticActors;
    lulu::Link link;
    std::unordered_map<std::string, Texture2D> textureCache;

  public:
    GameOn(const std::string &background, const std::string &music, const std::vector<std::string> &sounds,
           const std::vector<lulu::Key> &inputKeys, Game *game)
        : GameScene(background, music, sounds, inputKeys, game), arena(ARENA_POS, ARENA_SIZE),
          link(ARENA_POS + (ARENA_SIZE / 2), {50, 50}, 6, 12, 1, &arena)
    {
        //arena.spawn(lulu::BladeTrap(arena.pos()+arena.size()/2, {50, 50}, link.speed().x, 1, &arena, "core/assets/enemies/spines.png"));
        initializeStaticActors();
    }

    ~GameOn() override
    {
        cleanup();
    }

    void tick() override
    {
        arena.tick(getActiveKeys());
    }

    void render() override
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(_background, 0, 0, WHITE);

        const Texture2D linkTexture = getTexture(link.sprite());
        DrawTexture(linkTexture, static_cast<int>(link.pos().x), static_cast<int>(link.pos().y), WHITE);

        EndDrawing();
    }

  private:
    void initializeStaticActors()
    {
        staticActors.reserve(STATIC_ACTOR_POSITIONS.size());
        for (const auto &[x, y] : STATIC_ACTOR_POSITIONS)
        {
            const lulu::pair worldPos = ARENA_POS + lulu::pair{x * CELL_SIZE, y * CELL_SIZE};
            staticActors.emplace_back(worldPos, lulu::pair{CELL_SIZE, CELL_SIZE}, &arena);
        }
    }

    void cleanup()
    {
        for (auto *actor : arena.actors())
            arena.kill(actor);
        for (const auto &texture : textureCache | std::views::values)
            UnloadTexture(texture);
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

    static void drawRectOutline(const lulu::pair &pos, const lulu::pair &size, Color color)
    {
        DrawRectangleLines(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x),
                           static_cast<int>(size.y), color);
    }
};

class Game final
{
    const lulu::pair SCREEN_SIZE{800, 550};
    std::unique_ptr<GameScene> _scene;

  public:
    Game()
    {
        InitWindow(static_cast<int>(SCREEN_SIZE.x), static_cast<int>(SCREEN_SIZE.y), "The Legend of LuLù");
        SetTargetFPS(30);
        InitAudioDevice();

        // inizializzazione con una scena specifica
        _scene = std::make_unique<Menu>("core/assets/menu.png", "core/assets/sound/music/menu.mp3",
                                        std::vector<std::string>(), std::vector{lulu::K_SPACE}, this);
    }

    ~Game()
    {
        CloseAudioDevice();
        CloseWindow();
    }

    void run() const
    {
        while (!WindowShouldClose())
        {
            _scene->tick();
            _scene->render();
        }
    }

    void changeToGameScene()
    {
        std::vector keys = {lulu::K_W,    lulu::K_A,     lulu::K_S,  lulu::K_D,    lulu::K_DOWN,
                            lulu::K_LEFT, lulu::K_RIGHT, lulu::K_UP, lulu::K_SPACE};
        std::string hallPath = "core/assets/rooms/dungeon hall.png";
        _scene = std::make_unique<GameOn>(hallPath, "", std::vector<std::string>(), keys, this);
    }
};

// Implementazione del metodo tick di Menu (deve essere dopo la definizione di Game)
void Menu::tick()
{
    UpdateMusicStream(_music);
    for (const auto activeKeys = getActiveKeys(); const auto key : activeKeys)
    {
        if (key == lulu::K_SPACE)
        {
            _game->changeToGameScene();
            break;
        }
    }
}

int main()
{
    Game game;
    game.run();
    return 0;
}