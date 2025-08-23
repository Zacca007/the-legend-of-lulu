#pragma once
#include "types.hpp"

#include <memory>
#include <vector>

namespace lulu
{
class Actor;
class Arena final
{
  private:
    Vec2<int> pos_, size_;
    std::vector<Key> prevInputs_, currInputs_;
    /* Il vector contiene unique_ptr perché:
     * Gli oggetti Actor vengono creati nella funzione di parsing dal JSON.
     * Se li creassi in stack all’interno di un ciclo, non potrei passare i puntatori al vector senza rischiare
     * dangling pointer quando gli oggetti escono dallo scope.
     * Allocare manualmente in heap sarebbe possibile ma richiederebbe gestione esplicita della memoria, e non voglio
     * Con unique_ptr posso trasferire ownership in sicurezza, bypassando gli scope locali.
     * Inoltre, quando un unique_ptr perde ownership (es. rimosso dal vector o fine scope), l’oggetto viene automaticamente deallocato.
     * Infine, la sicurezza è maggiore perché esiste sempre un solo proprietario per ogni Actor e vive sempre solo nella lista.
     */
    std::vector<std::unique_ptr<Actor>> actors_;

    // internal methods for collisions
    void detectCollisionFor(Actor *actor);
    void detectCollision();

  public:
    Arena(Vec2<int> pos, Vec2<int> size);

    [[nodiscard]] const Vec2<int> &pos() const;
    [[nodiscard]] const Vec2<int> &size() const;
    [[nodiscard]] const std::vector<Key> &prevInputs() const;
    [[nodiscard]] const std::vector<Key> &currInputs() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Actor>> &actors() const;

    // main methods
    void spawn(std::unique_ptr<Actor> actor);
    void kill(Actor *actor);
    void tick(const std::vector<Key> &keys);

    // utility methods
    static bool hasKey(const std::vector<Key> &keys, Key key);
    bool isKeyJustPressed(Key key) const;
};
} // namespace lulu
