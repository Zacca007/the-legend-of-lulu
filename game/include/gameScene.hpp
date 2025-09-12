#pragma once
#include "lulu.hpp"
#include <raylib.h>
#include <string>
#include <vector>

namespace game
{
class Game;
class GameScene
{
  protected:
    Texture2D background_{};
    Music music_{};
    std::vector<lulu::Key> inputs_;
    Game *game_;

    [[nodiscard]] std::vector<lulu::Key> activeInputs() const;

    void setBackground(const std::string &configPath);
    void setMusic(const std::string &configPath);
    void setInputs(const std::string &configPath);

  public:
    GameScene(Game *game, const std::string &background, const std::string &music,
              const std::vector<lulu::Key> &inputs);
    GameScene(Game *game, const std::string &configPath);

    virtual ~GameScene();

    virtual void tick() = 0;
    virtual void render() = 0;
};
} // namespace game
