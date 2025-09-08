#include "arena.hpp"
#include "actor.hpp"
#include "fighters/link.hpp"
#include "utility actors/door.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <utility>

namespace lulu
{
Arena::Arena(const Vec2<float> pos, const Vec2<float> size) : pos_(pos), size_(size)
{
}

Arena::Arena(const std::string &configPath)
{
    std::ifstream f(configPath);
    if (!f.is_open())
    {
        throw std::runtime_error("Could not open config file: " + configPath + "from arena");
    }

    nlohmann::json j;
    f >> j;

    // sezione arena
    auto arenaJson = j.at("arena");

    // posizione dell'arena
    int arenaX = arenaJson.at("pos").at("x").get<int>();
    int arenaY = arenaJson.at("pos").at("y").get<int>();
    pos_ = Vec2{arenaX, arenaY}.convert<float>();

    // dimensioni dell'arena
    int arenaW = arenaJson.at("size").at("width").get<int>();
    int arenaH = arenaJson.at("size").at("height").get<int>();
    size_ = Vec2{arenaW, arenaH}.convert<float>();

    for (const auto &actorJson : arenaJson.at("actors"))
    {
        int ax = actorJson.at("pos").at("x").get<int>();
        int ay = actorJson.at("pos").at("y").get<int>();
        Vec2 pos{ax, ay};

        int aw = actorJson.at("size").at("width").get<int>();
        int ah = actorJson.at("size").at("height").get<int>();
        Vec2 size{aw, ah};

        auto actorPtr = std::make_unique<Actor>(pos.convert<float>(), size.convert<float>());
        spawn(std::move(actorPtr)); // ownership -> vector
    }

    for (const auto &doorJson : arenaJson.at("doors"))
    {
        Vec2 pos = {doorJson.at("pos").at("x").get<float>(), doorJson.at("pos").at("y").get<float>()};
        Vec2 size = {doorJson.at("size").at("width").get<float>(), doorJson.at("size").at("height").get<float>()};
        Vec2 spawn = {doorJson.at("spawn").at("x").get<float>(), doorJson.at("spawn").at("y").get<float>()};
        auto destination = doorJson.at("destination").get<std::string>();
        this->spawn(std::make_unique<Door>(pos, size, spawn, destination));
    }

    /*for (const auto &actorJson : arenaJson.at("characters"))
    {
        float x = actorJson.at("pos").at("x").get<float>();
        float y = actorJson.at("pos").at("y").get<float>();
        auto pos = Vec2{x, y};


         else if (type == "Zol")
         {
             auto zol = std::make_unique<Zol>(pos);
             collisions_[zol.get()] = {};
             spawn(std::move(zol));
         }
    }*/
}

const Vec2<float> &Arena::pos() const
{
    return pos_;
}

const Vec2<float> &Arena::size() const
{
    return size_;
}

const std::vector<Key> &Arena::prevInputs() const
{
    return prevInputs_;
}

const std::vector<Key> &Arena::currInputs() const
{
    return currInputs_;
}

const std::vector<std::unique_ptr<Actor>> &Arena::actors() const
{
    return actors_;
}

const std::unordered_map<const Actor *, std::vector<Collision>> &Arena::collisions() const
{
    return collisions_;
}

/* Prendo un unique_ptr per valore perché non è copiabile:
 * Il chiamante deve fare std::move() per trasferire ownership.
 * La funzione spawn sposta il puntatore nel vector, che diventa il nuovo proprietario.
 */
void Arena::spawn(std::unique_ptr<Actor> actor)
{
    if (!actor)
        return;

    actor->setArena(this);
    if (dynamic_cast<Movable *>(actor.get()))
    {
        collisions_[actor.get()] = {};
    }
    actors_.push_back(std::move(actor));
}

/* Per rimuovere un Actor dal vector:
 * Non posso passare unique ptr per valore perché non è copiabile.
 * Non posso usare move da fuori e passare ownership perchè l'oggetto da eliminare deve essere già dentro la lista.
 * Non posso passare una reference a unique ptr esistente nella lista perchè da fuori sono ritornati const dal getter.
 * Non voglio farli ritornare non const perchè sarebbe una pratica poco sicura.
 * Quindi passo un raw pointer e va bene così, tutti contenti.
 */
void Arena::kill(Actor *actor)
{
    if (!actor)
        return;

    // collisions_.erase(actor);
    for (auto &act : actors_)
    {
        if (act.get() == actor)
        {
            collisions_.erase(actor);
            std::erase(actors_, act);
            return;
        }
    }
}

void Arena::tick(const std::vector<Key> &keys)
{
    prevInputs_ = std::exchange(currInputs_, keys);
    for (const auto &act : actors_)
        if (auto *movable = dynamic_cast<Movable *>(act.get()))
        {
            movable->move();
            detectCollisionsFor(act.get());
            handleCollisionsFor(act.get());
        }
}

void Arena::detectCollisionsFor(const Actor *actor)
{
    auto &collisions = collisions_.at(actor);
    collisions.clear();

    // Check collision with all other actors
    for (auto &other : actors_)
    {
        if (actor == other.get())
            continue;

        // Check collision and add to collision list if found
        if (const auto coll = actor->checkCollision(other.get()); coll != D_NONE)
        {
            collisions.emplace_back(other.get(), coll);
        }
    }
}

void Arena::handleCollisionsFor(Actor *actor) const
{

    // Handle each collision for this actor
    for (const auto &actorCollisions = collisions_.at(actor); const auto &collision : actorCollisions)
    {
        actor->handleCollision(collision);
    }
}

bool Arena::hasKey(const std::vector<Key> &keys, const Key key)
{
    return std::ranges::find(keys, key) != keys.end();
}

bool Arena::isKeyJustPressed(const Key key) const
{

    const auto &currKeys = currInputs_;
    const auto &prevKeys = prevInputs_;

    return hasKey(currKeys, key) && !hasKey(prevKeys, key);
}
} // namespace lulu