#include "gameplay.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <print>

namespace game
{
Gameplay::Gameplay(Game *game, const std::string &configPath) : GameScene(game, configPath), arena_(parserArena(configPath))
{
}

lulu::Arena Gameplay::parserArena(const std::string &configPath)
{
    std::ifstream f(configPath);
    if (!f.is_open())
    {
        throw std::runtime_error("Could not open config file: " + configPath);
    }

    nlohmann::json j;
    f >> j;

    // sezione arena
    auto arenaJson = j.at("arena");

    // posizione dell'arena
    int arenaX = arenaJson.at("pos").at("x").get<int>();
    int arenaY = arenaJson.at("pos").at("y").get<int>();
    lulu::Vec2 arenaPos{arenaX, arenaY};

    // dimensioni dell'arena
    int arenaW = arenaJson.at("size").at("width").get<int>();
    int arenaH = arenaJson.at("size").at("height").get<int>();
    lulu::Vec2 arenaSize{arenaW, arenaH};

    lulu::Arena arena(arenaPos, arenaSize);

    // attori
    for (const auto &actorJson : arenaJson.at("actors"))
    {
        int ax = actorJson.at("pos").at("x").get<int>();
        int ay = actorJson.at("pos").at("y").get<int>();
        lulu::Vec2 pos{ax, ay};

        int aw = actorJson.at("size").at("width").get<int>();
        int ah = actorJson.at("size").at("height").get<int>();
        lulu::Vec2 size{aw, ah};

        auto actorPtr = std::make_unique<lulu::Actor>(pos, size);
        arena.spawn(std::move(actorPtr)); // ownership -> vector
    }

    return arena;
}


void Gameplay::tick()
{
    UpdateMusicStream(music_);
    std::print("the current number or actors is: {}\n", arena_.actors().size());
    for (const auto keys = activeInputs(); const auto key : keys)
    {
        //TODO: FIX THIS ASS CODE
        if (key == lulu::K_ENTER && !arena_.actors().empty())
        {
            const auto& ptr = arena_.actors()[0];
            arena_.kill(ptr);
        }
    }
}

void Gameplay::render()
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(background_, 0, 0, WHITE);

    DrawRectangleLines(arena_.pos().x, arena_.pos().y, arena_.size().x, arena_.size().y, WHITE);
    for (const auto &actor : arena_.actors())
    {
        DrawRectangleLines(actor->pos().x, actor->pos().y, actor->size().x, actor->size().y, WHITE);
    }

    EndDrawing();

}
} // namespace game