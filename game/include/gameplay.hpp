#pragma once
#include "gameScene.hpp"

namespace game
{
class Gameplay final : public GameScene
{
    lulu::Arena arena_;
    std::unordered_map<std::string, Texture2D> textureCache_;

    Texture2D getTexture(const std::string &path);

  public:
    explicit Gameplay(Game *game, const std::string &configPath = "assets/dungeon/configs/hall.json");
    ~Gameplay() override;

    void tick() override;
    void render() override;
};
} // namespace game