#include "actor.hpp"
namespace lulu
{
    class Character : public Actor{
        std::string name_;
        
        public:
        Character(Vec2<float> pos, Vec2<float> size, const std::string &name) : Actor(pos, size), name_(name) {}
        explicit Character(Vec2<float> pos, const std::string &configPath) : Actor(pos, configPath) {}
    };
} // namespace lulu
