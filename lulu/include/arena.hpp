#pragma once
#include "types.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

namespace lulu
{
class Actor;
class Arena final
{
  private:
    Vec2<float> pos_{}, size_{};
    std::vector<Key> prevInputs_, currInputs_;
    /* Il vector contiene unique_ptr perché:
     * Gli oggetti Actor vengono creati nella funzione di parsing dal JSON.
     * Se li creassi in stack all’interno di un ciclo, non potrei passare i puntatori al vector senza rischiare
     * dangling pointer quando gli oggetti escono dallo scope.
     * Allocare manualmente in heap sarebbe possibile ma richiederebbe gestione esplicita della memoria, e non voglio
     * Con unique_ptr posso trasferire ownership in sicurezza, bypassando gli scope locali.
     * Inoltre, quando un unique_ptr perde ownership (es. Rimosso dal vector o fine scope), l’oggetto viene
     * automaticamente liberato. Infine, la sicurezza è maggiore perché esiste sempre un solo proprietario per ogni
     * Actor e vive sempre solo nella lista.
     */
    std::vector<std::unique_ptr<Actor>> actors_;
    std::unordered_map<const Actor *, std::vector<Collision>> collisions_; // Collision data per actor

    // internal methods for collisions
    void detectCollisionsFor(const Actor *actor);
    void handleCollisionsFor(Actor *actor) const;

  public:
    Arena(Vec2<float> pos, Vec2<float> size);
    explicit Arena(const std::string &configPath);

    [[nodiscard]] const Vec2<float> &pos() const;
    [[nodiscard]] const Vec2<float> &size() const;
    [[nodiscard]] const std::vector<Key> &prevInputs() const;
    [[nodiscard]] const std::vector<Key> &currInputs() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Actor>> &actors() const;
    [[nodiscard]] const std::unordered_map<const Actor *, std::vector<Collision>> &collisions() const;

    // main methods
    void spawn(std::unique_ptr<Actor> actor);
    std::unique_ptr<Actor> kill(Actor *actor);
    void tick(const std::vector<Key> &keys);

    // utility methods
    static bool hasKey(const std::vector<Key> &keys, Key key);
    [[nodiscard]] bool isKeyJustPressed(Key key) const;
};
} // namespace lulu
