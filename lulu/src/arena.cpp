#include "arena.hpp"
#include "actor.hpp"
#include "movable.hpp"
#include "utility actors/door.hpp"
#include "utility actors/npc.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <utility>

#include "fighters/zol.hpp"

namespace lulu
{
    Arena::Arena(const Vec2<float> pos, const Vec2<float> size) : pos_(pos), size_(size)
    {
    }

    // Helper privato per parsing Vec2 dal JSON
    namespace
    {
        Vec2<float> parseVec2(const nlohmann::json& j)
        {
            return Vec2{
                j.at("x").get<float>(),
                j.at("y").get<float>()
            };
        }

        Vec2<float> parseSize(const nlohmann::json& j)
        {
            return Vec2{
                j.at("width").get<float>(),
                j.at("height").get<float>()
            };
        }
    }

    Arena::Arena(const std::string& configPath)
    {
        std::ifstream f(configPath);
        if (!f.is_open())
        {
            throw std::runtime_error("Could not open config file: " + configPath);
        }

        nlohmann::json j;
        f >> j;

        const auto& arenaJson = j.at("arena");

        // Parse arena properties
        pos_ = parseVec2(arenaJson.at("pos"));
        size_ = parseSize(arenaJson.at("size"));

        // Load actors
        loadActors(arenaJson);
        loadEnemies(arenaJson);
        loadDoors(arenaJson);
        loadNPCs(arenaJson);
    }

    void Arena::loadActors(const nlohmann::json& arenaJson)
    {
        if (!arenaJson.contains("actors")) return;

        for (const auto& actorJson : arenaJson.at("actors"))
        {
            Vec2<float> pos = parseVec2(actorJson.at("pos"));
            Vec2<float> size = parseSize(actorJson.at("size"));

            spawn(std::make_unique<Actor>(pos, size));
        }
    }

    void Arena::loadEnemies(const nlohmann::json& arenaJson)
    {
        if (!arenaJson.contains("enemies")) return;

        for (const auto& enemyJson : arenaJson.at("enemies"))
        {
            auto type = enemyJson.at("type").get<std::string>();
            Vec2<float> pos = parseVec2(enemyJson.at("pos"));

            if (type == "zol")
            {
                spawn(std::make_unique<Zol>(pos));
            }
            // Qui puoi aggiungere altri tipi di nemici in futuro:
            // else if (type == "moblin") { spawn(std::make_unique<Moblin>(pos, config)); }
        }
    }

    void Arena::loadDoors(const nlohmann::json& arenaJson)
    {
        if (!arenaJson.contains("doors")) return;

        for (const auto& doorJson : arenaJson.at("doors"))
        {
            Vec2<float> pos = parseVec2(doorJson.at("pos"));
            Vec2<float> size = parseSize(doorJson.at("size"));
            Vec2<float> spawnPos = parseVec2(doorJson.at("spawn"));
            bool changeMusic = doorJson.at("changeMusic").get<bool>();
            auto destination = doorJson.at("destination").get<std::string>();

            spawn(std::make_unique<Door>(pos, size, spawnPos, destination, changeMusic));
        }
    }

    void Arena::loadNPCs(const nlohmann::json& arenaJson)
    {
        if (!arenaJson.contains("NPCs")) return;

        for (const auto& npcJson : arenaJson.at("NPCs"))
        {
            Vec2<float> pos = parseVec2(npcJson.at("pos"));
            Vec2<float> size = parseSize(npcJson.at("size"));
            auto name = npcJson.at("name").get<std::string>();
            auto sprite = npcJson.at("sprite").get<std::string>();
            auto dialogue = npcJson.at("dialoguePath").get<std::string>();

            spawn(std::make_unique<NPC>(pos, size, sprite, dialogue, name));
        }
    }

    // Resto dei metodi invariati...
    const Vec2<float>& Arena::pos() const { return pos_; }
    const Vec2<float>& Arena::size() const { return size_; }
    const std::vector<Key>& Arena::prevInputs() const { return prevInputs_; }
    const std::vector<Key>& Arena::currInputs() const { return currInputs_; }
    const std::vector<std::unique_ptr<Actor>>& Arena::actors() const { return actors_; }

    const std::unordered_map<const Actor*, std::vector<Collision>>& Arena::collisions() const
    {
        return collisions_;
    }

    void Arena::tick(const std::vector<Key>& keys)
    {
        prevInputs_ = std::exchange(currInputs_, keys);

        for (const auto& act : actors_)
        {
            if (auto* movable = dynamic_cast<Movable*>(act.get()))
            {
                movable->move();
                detectCollisionsFor(act.get());
                handleCollisionsFor(act.get());

                if (const auto* fighter = dynamic_cast<Fighter*>(act.get()))
                {
                    if (fighter->shouldDie())
                    {
                        kill(act.get());
                    }
                }
            }
        }
    }

    void Arena::spawn(std::unique_ptr<Actor> actor)
    {
        if (!actor) return;

        actor->setArena(this);
        if (dynamic_cast<Movable*>(actor.get()))
        {
            collisions_[actor.get()] = {};
        }
        actors_.push_back(std::move(actor));
    }

    std::unique_ptr<Actor> Arena::kill(Actor* actor)
    {
        if (!actor) return nullptr;

        const auto it = std::ranges::find_if(actors_,
                                             [actor](const auto& a) { return a.get() == actor; });

        if (it != actors_.end())
        {
            collisions_.erase(actor);
            std::unique_ptr<Actor> extracted = std::move(*it);
            actors_.erase(it);
            return extracted;
        }

        return nullptr;
    }

    void Arena::detectCollisionsFor(const Actor* actor)
    {
        auto& collisions = collisions_.at(actor);
        collisions.clear();

        for (auto& other : actors_)
        {
            if (actor == other.get()) continue;

            if (const auto coll = actor->checkCollision(other.get()); coll != D_NONE)
            {
                collisions.emplace_back(other.get(), coll);
            }
        }
    }

    void Arena::handleCollisionsFor(Actor* actor) const
    {
        for (const auto& collision : collisions_.at(actor))
        {
            actor->handleCollision(collision);
        }
    }

    bool Arena::hasKey(const std::vector<Key>& keys, const Key key)
    {
        return std::ranges::find(keys, key) != keys.end();
    }

    bool Arena::isKeyJustPressed(const Key key) const
    {
        return hasKey(currInputs_, key) && !hasKey(prevInputs_, key);
    }
}
