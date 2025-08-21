#include "lulu.hpp"
#include <iostream>
#include <memory>
#include <ranges>
#include <raylib.h>
#include <unordered_map>
#include <vector>

class Game;

class GameScene
{
  protected:
    Texture2D _background{};
    Music _music{};
    std::map<std::string, Sound> _sounds;
    std::vector<lulu::Key> _inputKeys;
    Game *_game{nullptr};

  public:
    GameScene(const std::string &background, const std::string &music, const std::vector<std::string> &sounds,
              const std::vector<lulu::Key> &inputKeys, Game *game = nullptr)
        : _inputKeys(inputKeys), _game(game)
    {
        if (!background.empty())
            _background = LoadTexture(background.c_str());
        if (!music.empty())
        {
            _music = LoadMusicStream(music.c_str());
            PlayMusicStream(_music);
        }
        for (const auto &s : sounds)
            _sounds[s] = LoadSound(s.c_str());
    }

    virtual ~GameScene()
    {
        UnloadTexture(_background);
        UnloadMusicStream(_music);
        for (auto &sound : _sounds | std::views::values)
            UnloadSound(sound);
    }

    [[nodiscard]] std::vector<lulu::Key> getActiveKeys() const
    {
        std::vector<lulu::Key> keys;
        for (auto k : _inputKeys)
            if (IsKeyDown(k))
                keys.push_back(k);
        return keys;
    }

    virtual void tick() = 0;
    virtual void render() = 0;
};

class Menu final : public GameScene
{
    unsigned char transparency{0};
    bool growing{true};

  public:
    Menu(const std::string &background, const std::string &music, const std::vector<std::string> &sounds,
         const std::vector<lulu::Key> &inputKeys, Game *game)
        : GameScene(background, music, sounds, inputKeys, game)
    {
        _music.looping = false;
    }

    void tick() override;
    void render() override
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(_background, 0, 0, WHITE);
        DrawText("Press SPACE to start", 225, 500, 30, {255, 255, 255, transparency});
        EndDrawing();

        transparency += growing ? 15 : -15;
        if (transparency >= 255)
            growing = false;
        else if (transparency <= 0)
            growing = true;
    }
};

class GameOn final : public GameScene
{
    lulu::Arena arena;
    std::unique_ptr<lulu::Link> link;
    std::vector<std::unique_ptr<lulu::Actor>> roomActors;
    std::unordered_map<std::string, Texture2D> textureCache;
    lulu::RoomConfig roomConfig;

  public:
    GameOn(const std::string &roomConfigPath, const std::vector<lulu::Key> &inputKeys, Game *game)
        : GameScene("", "", {}, inputKeys, game), arena({0, 0}, {0, 0})
    {
        loadRoomFromConfig(roomConfigPath);
    }

    ~GameOn() override
    {
        cleanup();
    }

    void tick() override
    {
        UpdateMusicStream(_music);
        arena.tick(getActiveKeys());
    }

    void render() override
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(_background, 0, 0, WHITE);

        if (link)
        {
            const Texture2D tex = getTexture(link->sprite());
            DrawTexture(tex, static_cast<int>(link->pos().x), static_cast<int>(link->pos().y), WHITE);
        }

        for (const auto &actor : roomActors)
        {
            if (!actor->sprite().empty())
            {
                const Texture2D tex = getTexture(actor->sprite());
                DrawTexture(tex, static_cast<int>(actor->pos().x), static_cast<int>(actor->pos().y), WHITE);
            }
        }

        EndDrawing();
    }

  private:
    void loadRoomFromConfig(const std::string &configPath)
    {
        roomConfig = lulu::RoomLoader::loadRoom(configPath);
        arena = lulu::Arena(roomConfig.arenaPosition, roomConfig.arenaSize);

        if (!roomConfig.background.empty())
            _background = LoadTexture(roomConfig.background.c_str());
        if (!roomConfig.music.empty())
        {
            _music = LoadMusicStream(roomConfig.music.c_str());
            PlayMusicStream(_music);
        }
        for (const auto &s : roomConfig.sounds)
            _sounds[s] = LoadSound(s.c_str());

        link =
            std::make_unique<lulu::Link>(roomConfig.playerSpawnPosition, roomConfig.playerSize, roomConfig.playerSpeed,
                                         roomConfig.playerHp, roomConfig.playerDamage, &arena);

        roomActors = lulu::RoomLoader::createActorsFromConfig(roomConfig, &arena);
        std::cout << "Loaded room: " << roomConfig.name << " | Actors: " << roomActors.size() << std::endl;
    }

    void cleanup()
    {
        for (auto &actor : roomActors)
            arena.kill(actor.get());
        if (link)
            arena.kill(link.get());
        for (const auto &tex : textureCache | std::views::values)
            UnloadTexture(tex);
    }

    Texture2D getTexture(const std::string &path)
    {
        if (path.empty())
            return {};
        return textureCache.try_emplace(path, LoadTexture(path.c_str())).first->second;
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
        _scene = std::make_unique<Menu>("assets/menu.png", "assets/sound/music/menu.mp3", std::vector<std::string>(),
                                        std::vector{lulu::K_SPACE}, this);
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
        _scene = std::make_unique<GameOn>("assets/rooms/dungeon_hall.json",
                                          std::vector{lulu::K_W, lulu::K_A, lulu::K_S, lulu::K_D, lulu::K_DOWN,
                                                      lulu::K_LEFT, lulu::K_RIGHT, lulu::K_UP, lulu::K_SPACE},
                                          this);
    }
};

void Menu::tick()
{
    UpdateMusicStream(_music);
    for (auto key : getActiveKeys())
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
    const Game game;
    game.run();
    return 0;
}
