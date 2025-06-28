#pragma once
#include "actors/fighter.hpp"

namespace lulu
{

    class Link : public Fighter
    {
    public:
        Link(pair position, pair size, pair speed, Arena *arena = nullptr, const std::string &sprite = "")
            : Fighter(position, size, speed, arena, sprite) {};
        
        void move() override;
    };
}