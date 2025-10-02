#pragma once
#include <string>
#include <vector>
#include <memory>
#include "raylib.h"
#include "utility actors/npc.hpp"

namespace game
{
    class DialogueManager {
    private:
        const lulu::NPC* currentNPC_ = nullptr;
        std::vector<lulu::DialogueLine> lines_;
        size_t currentLineIndex_ = 0;
        bool isActive_ = false;

        // Animazione testo
        float textTimer_ = 0.0f;
        float textSpeed_ = 30.0f;
        int currentCharIndex_ = 0;
        bool isTextComplete_ = false;

        // Texture del box - caricata UNA volta
        Texture2D dialogueBoxTexture_;
        bool textureLoaded_ = false;

    public:
        DialogueManager();
        ~DialogueManager();

        void startDialogue(const lulu::NPC* npc, const std::vector<lulu::DialogueLine>& dialogueLines);
        void update(float deltaTime);
        void render() const;
        void advance();
        void reset();
        void completeText();

        bool isActive() const { return isActive_ && currentLineIndex_ < lines_.size(); }
        bool hasMoreLines() const { return currentLineIndex_ + 1 < lines_.size(); }
        bool isTextComplete() const { return isTextComplete_; }

        std::string getCurrentText() const;
        std::string getCurrentSpeaker() const;
        std::string getCurrentPortrait() const;
    };
}