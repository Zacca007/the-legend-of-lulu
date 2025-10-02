#pragma once
#include "types.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

namespace lulu
{
  class Actor;

  class Arena final
  {
    Vec2<float> pos_{};
    Vec2<float> size_{};
    std::vector<Key> prevInputs_, currInputs_;
    std::vector<std::unique_ptr<Actor>> actors_;
    std::unordered_map<const Actor*, std::vector<Collision>> collisions_;

    void detectCollisionsFor(const Actor* actor);
    void handleCollisionsFor(Actor* actor) const;

    // Helper per parsing JSON
    void loadActors(const nlohmann::json& arenaJson);
    void loadDoors(const nlohmann::json& arenaJson);
    void loadNPCs(const nlohmann::json& arenaJson);

  public:
    Arena(Vec2<float> pos, Vec2<float> size);
    explicit Arena(const std::string& configPath);

    [[nodiscard]] const Vec2<float>& pos() const;
    [[nodiscard]] const Vec2<float>& size() const;
    [[nodiscard]] const std::vector<Key>& prevInputs() const;
    [[nodiscard]] const std::vector<Key>& currInputs() const;
    [[nodiscard]] const std::vector<std::unique_ptr<Actor>>& actors() const;
    [[nodiscard]] const std::unordered_map<const Actor*, std::vector<Collision>>& collisions() const;

    void spawn(std::unique_ptr<Actor> actor);
    std::unique_ptr<Actor> kill(Actor* actor);
    void tick(const std::vector<Key>& keys);

    static bool hasKey(const std::vector<Key>& keys, Key key);
    [[nodiscard]] bool isKeyJustPressed(Key key) const;
  };
}
