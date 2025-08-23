#include "arena.hpp"
#include "actor.hpp"

namespace lulu
{
Arena::Arena(const Vec2<int> pos, const Vec2<int> size) : pos_(pos), size_(size)
{
}

const Vec2<int> &Arena::pos() const
{
    return pos_;
}

const Vec2<int> &Arena::size() const
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

/* Prendo un unique_ptr per valore perché non è copiabile:
 * Il chiamante deve fare std::move() per trasferire ownership.
 * La funzione spawn sposta il puntatore nel vector, che diventa il nuovo proprietario.
 */
void Arena::spawn(std::unique_ptr<Actor> actor)
{
    if (!actor) return;

    // Se l'attore appartiene già a un'altra arena, rimuovilo da lì
    if (actor->arena() && actor->arena() != this)
    {
        actor->arena()->kill(actor.get());
    }

    // Imposta l'arena sull'Actor
    actor->setArena(this);

    // Aggiungi al vector (ownership trasferita)
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
    for (auto& act : actors_)
    {
        if (act.get() == actor)
        {
            std::erase(actors_, act);
            act->setArena(nullptr);
        }
    }
}

void Arena::tick(const std::vector<Key> &keys)
{
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