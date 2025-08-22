#pragma once

#include <raylib.h>
#include <string>
#include <vector>

namespace game
{
// TODO: replace int with lulu::key
class GameScene
{
  protected:
    Texture2D background_;
    Music music_;
    std::vector<int> inputs_;

    std::vector<int> activeInputs() const;

  public:
    GameScene(const std::string &background, const std::string &music, const std::vector<int> &inputs);
    virtual ~GameScene();

    virtual void tick() = 0;
    virtual void render() = 0;
};
} // namespace game
