#include "arena.hpp"
#include "actor.hpp"
#include "movable.hpp"
#include <algorithm>
#include <set>
using lulu::Arena;

void Arena::spawn(Actor *actor)
{
    if (!actor)
        return;

    actor->setArena(this);

    // Evita di aggiungere lo stesso attore due volte
    for (Actor *currentActor : _actors)
    {
        if (currentActor == actor)
            return;
    }
    _actors.push_back(actor);
    _collision.insert({actor, std::vector<Actor *>()});
}

void Arena::kill(Actor *actor)
{
    if (!actor)
        return;

    _collision.erase(actor);
    std::erase(_actors, actor);
    actor->setArena(nullptr);
}

void Arena::tick(const std::vector<Key> &keys)
{
    // Crea una copia degli attori in ordine inverso (come in Python)
    std::vector<Actor *> actorsReversed = _actors;
    std::reverse(actorsReversed.begin(), actorsReversed.end());

    // Rileva le collisioni prima di muovere gli attori
    detectCollisions(actorsReversed);

    // Aggiorna i tasti
    _prevKeys = _currKeys;
    _currKeys = keys;

    // Muove tutti gli attori
    for (Actor *actor : actorsReversed)
    {
        Movable *movable = dynamic_cast<Movable *>(actor);
        if (movable)
        {
            movable->move();
        }
    }

    _count++;
}

void Arena::detectCollisions(std::vector<Actor *> &actors)
{
    // Pulisce le collisioni precedenti
    for (auto &collision : _collision)
    {
        collision.second.clear();
    }

    const int tile = 40; // Dimensione della griglia (come in Python)

    // Calcola le dimensioni della griglia usando ceiling division
    int nx = (-(-_size.x / tile)); // Equivalente a -(-self._w // tile) in Python
    int ny = (-(-_size.y / tile)); // Equivalente a -(-self._h // tile) in Python

    // Crea la griglia di celle, ogni cella contiene gli indici degli attori
    std::vector<std::set<int>> cells(nx * ny);

    // Prima fase: popola la griglia con gli indici degli attori
    for (std::size_t i = 0; i < actors.size(); i++)
    {
        Actor *actor = actors[i];
        if (!actor)
            continue;

        pair pos = actor->pos();
        pair size = actor->size();

        int x = pos.x, y = pos.y;
        int w = size.x, h = size.y;

        // Calcola le celle occupate dall'attore (con margine di 1 pixel)
        for (int tx = (x - 1) / tile; tx <= (x + w + 1) / tile; tx++)
        {
            for (int ty = (y - 1) / tile; ty <= (y + h + 1) / tile; ty++)
            {
                if (tx >= 0 && tx < nx && ty >= 0 && ty < ny)
                {
                    cells[ty * nx + tx].insert(i);
                }
            }
        }
    }

    // Seconda fase: trova le collisioni per ogni attore
    for (std::size_t i = 0; i < actors.size(); i++)
    {
        Actor *actor = actors[i];
        if (!actor)
            continue;

        std::set<int> neighbors;
        pair pos = actor->pos();
        pair size = actor->size();

        int x = pos.x, y = pos.y;
        int w = size.x, h = size.y;

        // Raccoglie tutti i vicini nelle celle adiacenti
        for (int tx = (x - 1) / tile; tx <= (x + w + 1) / tile; tx++)
        {
            for (int ty = (y - 1) / tile; ty <= (y + h + 1) / tile; ty++)
            {
                if (tx >= 0 && tx < nx && ty >= 0 && ty < ny)
                {
                    const std::set<int> &cellActors = cells[ty * nx + tx];
                    neighbors.insert(cellActors.begin(), cellActors.end());
                }
            }
        }

        std::vector<int> neighborIndices(neighbors.begin(), neighbors.end());
        std::sort(neighborIndices.rbegin(), neighborIndices.rend()); // Ordine decrescente

        std::vector<Actor *> collisions;
        for (std::size_t j : neighborIndices)
        {
            if (i != j && actors[j])
            {
                collision collType = actor->checkCollision(actors[j]);
                if (collType != C_NONE)
                {
                    collisions.push_back(actors[j]);
                }
            }
        }

        // Aggiorna la mappa delle collisioni
        _collision[actor] = collisions;
    }
}