#pragma once
#include "actor.hpp"
#include <string>
#include <vector>

namespace lulu
{
    struct DialogueLine
    {
        std::string speaker;
        std::string text;
        std::string portrait;
    };

    class NPC final: public Actor
    {
    private:
        std::string dialoguePath_;
        mutable std::vector<DialogueLine> cachedDialogue_;
        mutable bool dialogueLoaded_ = false;
        std::string name_;

    public:
        NPC(const Vec2<float>& pos, const Vec2<float>& size, const std::string& sprite,
    const std::string& dialoguePath, const std::string& name);

        std::vector<DialogueLine> loadDialogue() const;
        const std::string& name() const { return name_; }
        const std::string& dialoguePath() const { return dialoguePath_; }
    };
}