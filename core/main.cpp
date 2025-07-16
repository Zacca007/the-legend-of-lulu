#include "lulu.hpp"
#include "roomLoader.hpp"
#include <array>
#include <memory>
#include <ranges>
#include <raylib.h>
#include <unordered_map>
#include <vector>
#include <iostream>

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
        PlayMusicStream(_music);
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
    lulu::Arena arena;
    std::unique_ptr<lulu::Link> link;
    std::vector<std::unique_ptr<lulu::Actor>> roomActors;
    std::unordered_map<std::string, Texture2D> textureCache;
    lulu::RoomConfig roomConfig;

  public:
    GameOn(const std::string& roomConfigPath, const std::vector<lulu::Key> &inputKeys, Game *game)
        : GameScene("", "", {}, inputKeys, game), arena({0, 0}, {0, 0})
    {
        try {
            loadRoomFromConfig(roomConfigPath);
        } catch (const std::exception& e) {
            std::cerr << "Error loading room: " << e.what() << std::endl;
            // Fallback to hardcoded values
            createFallbackRoom();
        }
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

        // Render Link
        if (link) {
            const Texture2D linkTexture = getTexture(link->sprite());
            DrawTexture(linkTexture, static_cast<int>(link->pos().x), static_cast<int>(link->pos().y), WHITE);
        }

        // Optionally render other actors with sprites
        for (const auto& actor : roomActors) {
            if (!actor->sprite().empty()) {
                const Texture2D actorTexture = getTexture(actor->sprite());
                DrawTexture(actorTexture, static_cast<int>(actor->pos().x), static_cast<int>(actor->pos().y), WHITE);
            }
        }

        EndDrawing();
    }

  private:
    void loadRoomFromConfig(const std::string& configPath) {
        // Load room configuration from JSON
        roomConfig = lulu::RoomLoader::loadRoom(configPath);

        // Initialize arena with loaded configuration
        arena = lulu::Arena(roomConfig.arenaPosition, roomConfig.arenaSize);

        // Load background and music
        if (!roomConfig.background.empty()) {
            _background = LoadTexture(roomConfig.background.c_str());
        }
        if (!roomConfig.music.empty()) {
            _music = LoadMusicStream(roomConfig.music.c_str());
            PlayMusicStream(_music);
        }

        // Load sounds
        for (const auto& soundPath : roomConfig.sounds) {
            _sounds[soundPath] = LoadSound(soundPath.c_str());
        }

        // Create Link with loaded configuration
        link = std::make_unique<lulu::Link>(
            roomConfig.playerSpawnPosition,
            roomConfig.playerSize,
            roomConfig.playerSpeed,
            roomConfig.playerHp,
            roomConfig.playerDamage,
            &arena
        );

        // Create room actors from configuration
        roomActors = lulu::RoomLoader::createActorsFromConfig(roomConfig, &arena);

        std::cout << "Successfully loaded room: " << roomConfig.name << std::endl;
        std::cout << "Static actors: " << roomActors.size() << std::endl;
    }

    void createFallbackRoom() {
        // Fallback to original hardcoded room if JSON loading fails
        std::cout << "Using fallback room configuration" << std::endl;

        const lulu::pair ARENA_SIZE{600, 350};
        const lulu::pair ARENA_POS{100, 100};
        const int CELL_SIZE = 50;
        const std::array<lulu::pair, 12> STATIC_ACTOR_POSITIONS = {
            {{1, 1}, {1, 3}, {1, 5}, {4, 1}, {4, 3}, {4, 5}, {7, 1}, {7, 3}, {7, 5}, {10, 1}, {10, 3}, {10, 5}}};

        arena = lulu::Arena(ARENA_POS, ARENA_SIZE);
        _background = LoadTexture("assets/rooms/dungeon hall.png");
        _music = LoadMusicStream("assets/sound/music/room.mp3");
        PlayMusicStream(_music);

        link = std::make_unique<lulu::Link>(ARENA_POS + ARENA_SIZE / 2, lulu::pair{50, 50}, 6, 12, 1, &arena);

        // Create static actors
        roomActors.reserve(STATIC_ACTOR_POSITIONS.size());
        for (const auto &[x, y] : STATIC_ACTOR_POSITIONS)
        {
            const lulu::pair worldPos = ARENA_POS + lulu::pair{x * CELL_SIZE, y * CELL_SIZE};
            roomActors.push_back(std::make_unique<lulu::Actor>(worldPos, lulu::pair{CELL_SIZE, CELL_SIZE}, &arena));
        }
    }

    void cleanup()
    {
        // Clear actors from arena
        for (const auto& actor : roomActors) {
            arena.kill(actor.get());
        }
        if (link) {
            arena.kill(link.get());
        }

        // Unload textures
        for (const auto &texture : textureCache | std::views::values)
            UnloadTexture(texture);
    }

    Texture2D getTexture(const std::string &path)
    {
        if (path.empty()) {
            // Return a default empty texture or handle appropriately
            return {};
        }

        auto it = textureCache.find(path);
        if (it == textureCache.end())
        {
            textureCache[path] = LoadTexture(path.c_str());
            return textureCache[path];
        }
        return it->second;
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
        _scene = std::make_unique<Menu>("assets/menu.png", "assets/sound/music/menu.mp3",
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

        // Use JSON configuration file instead of hardcoded values
        _scene = std::make_unique<GameOn>("assets/rooms/dungeon_hall.json", keys, this);
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
    const Game game;
    game.run();
    return 0;
}