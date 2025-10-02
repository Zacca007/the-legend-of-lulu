// dialogueManager.cpp - Versione refactored

#include "dialogueManager.hpp"
#include <raylib.h>
#include <iostream>
#include <unordered_map>

namespace game
{
        DialogueManager::DialogueManager()
    {
        // Carica la texture UNA volta sola
        dialogueBoxTexture_ = LoadTexture("assets/ui/dialoguebox.png");
        if (dialogueBoxTexture_.id == 0)
        {
            std::cout << "ERRORE: Impossibile caricare assets/ui/dialoguebox.png" << std::endl;
            // Crea un placeholder
            Image fallback = GenImageColor(600, 200, Color{0, 0, 0, 200});
            dialogueBoxTexture_ = LoadTextureFromImage(fallback);
            UnloadImage(fallback);
        }
        else
        {
            std::cout << "Dialogue box texture caricata: "
                << dialogueBoxTexture_.width << "x" << dialogueBoxTexture_.height << std::endl;
            textureLoaded_ = true;
        }
    }

    DialogueManager::~DialogueManager()
    {
        if (textureLoaded_)
        {
            UnloadTexture(dialogueBoxTexture_);
        }
    }

    void DialogueManager::startDialogue(const lulu::NPC* npc, const std::vector<lulu::DialogueLine>& dialogueLines)
    {
        currentNPC_ = npc;
        lines_ = dialogueLines;
        currentLineIndex_ = 0;
        isActive_ = true;
        currentCharIndex_ = 0;
        textTimer_ = 0.0f;
        isTextComplete_ = false;
    }

    void DialogueManager::update(float deltaTime)
    {
        if (isTextComplete_ || lines_.empty()) return;

        textTimer_ += deltaTime;

        if (int targetChars = static_cast<int>(textTimer_ * textSpeed_); targetChars >= lines_[currentLineIndex_].text.
            length())
        {
            completeText();
        }
        else
        {
            currentCharIndex_ = targetChars;
        }
    }

    void DialogueManager::advance()
    {
        if (!isTextComplete_)
        {
            completeText();
        }
        else if (currentLineIndex_ + 1 < lines_.size())
        {
            currentLineIndex_++;
            currentCharIndex_ = 0;
            textTimer_ = 0.0f;
            isTextComplete_ = false;
        }
        else
        {
            isActive_ = false;
        }
    }

    void DialogueManager::reset()
    {
        currentNPC_ = nullptr;
        lines_.clear();
        currentLineIndex_ = 0;
        isActive_ = false;
        textTimer_ = 0.0f;
        currentCharIndex_ = 0;
        isTextComplete_ = false;
    }

    void DialogueManager::completeText()
    {
        if (!lines_.empty())
        {
            currentCharIndex_ = lines_[currentLineIndex_].text.length();
        }
        isTextComplete_ = true;
    }

    std::string DialogueManager::getCurrentText() const
    {
        if (currentLineIndex_ < lines_.size())
        {
            return lines_[currentLineIndex_].text.substr(0, currentCharIndex_);
        }
        return "";
    }

    std::string DialogueManager::getCurrentSpeaker() const
    {
        if (currentLineIndex_ < lines_.size())
        {
            return lines_[currentLineIndex_].speaker;
        }
        return "";
    }

    std::string DialogueManager::getCurrentPortrait() const
    {
        if (currentLineIndex_ < lines_.size())
        {
            return lines_[currentLineIndex_].portrait;
        }
        return "";
    }

    namespace
    {
        // Costanti estratte per migliore manutenibilità
        constexpr int BOX_X = 100;
        constexpr int BOX_Y = 300;
        constexpr int PORTRAIT_SIZE = 100;
        constexpr int PORTRAIT_PADDING = 20;
        constexpr int TEXT_START_X = 150;
        constexpr int TEXT_START_Y = 60;
        constexpr int SPEAKER_FONT_SIZE = 24;
        constexpr int TEXT_FONT_SIZE = 22;
        constexpr int LINE_HEIGHT = 30;
        constexpr int MAX_TEXT_WIDTH = 445;
        constexpr int MAX_VISIBLE_LINES = 3;

        // Helper per wrapping del testo
        std::vector<std::string> wrapText(const std::string& text, int fontSize, int maxWidth)
        {
            std::vector<std::string> lines;
            std::string currentLine;

            for (char c : text)
            {
                currentLine += c;
                int textWidth = MeasureText(currentLine.c_str(), fontSize);

                if (textWidth >= maxWidth)
                {
                    size_t lastSpace = currentLine.find_last_of(' ');
                    if (lastSpace != std::string::npos && lastSpace > 0)
                    {
                        lines.push_back(currentLine.substr(0, lastSpace));
                        currentLine = currentLine.substr(lastSpace + 1);
                    }
                    else
                    {
                        lines.push_back(currentLine);
                        currentLine.clear();
                    }
                }
            }

            if (!currentLine.empty())
            {
                lines.push_back(currentLine);
            }

            return lines;
        }

        void renderPortrait(const std::string& portraitPath, int x, int y)
        {
            if (portraitPath.empty()) return;

            // Cache della texture invece di ricaricarla ogni frame
            static std::unordered_map<std::string, Texture2D> portraitCache;

            if (!portraitCache.contains(portraitPath))
            {
                Texture2D portrait = LoadTexture(portraitPath.c_str());
                if (portrait.id != 0)
                {
                    portraitCache[portraitPath] = portrait;
                }
                else
                {
                    std::cout << "ERRORE: Impossibile caricare ritratto: " << portraitPath << std::endl;
                    return;
                }
            }

            DrawTexture(portraitCache[portraitPath], x, y, WHITE);
        }

        void renderPlaceholderPortrait(int x, int y, int size)
        {
            DrawRectangle(x, y, size, size, Color{50, 50, 80, 200});
            DrawRectangleLines(x, y, size, size, BLUE);
            DrawText("?", x + 40, y + 40, 30, WHITE);
            DrawText("No img", x + 10, y + size + 10, 12, LIGHTGRAY);
        }
    }

    void DialogueManager::render() const
    {
        if (!isActive_) return;

        const int boxWidth = dialogueBoxTexture_.width;
        const int boxHeight = dialogueBoxTexture_.height;

        // 1. Background semi-trasparente
        DrawRectangle(BOX_X, BOX_Y, boxWidth, boxHeight, Color{30, 60, 120, 180});

        // 2. Texture del dialogue box
        DrawTexture(dialogueBoxTexture_, BOX_X, BOX_Y, WHITE);

        // 3. Speaker name
        const std::string speaker = getCurrentSpeaker();
        if (!speaker.empty())
        {
            DrawText(speaker.c_str(), BOX_X + TEXT_START_X, BOX_Y + 15,
                     SPEAKER_FONT_SIZE, WHITE);
        }

        // 4. Wrapped text
        const std::string currentText = getCurrentText();
        std::vector<std::string> lines = wrapText(currentText, TEXT_FONT_SIZE, MAX_TEXT_WIDTH);

        for (size_t i = 0; i < lines.size() && i < MAX_VISIBLE_LINES; i++)
        {
            DrawText(lines[i].c_str(),
                     BOX_X + TEXT_START_X,
                     BOX_Y + TEXT_START_Y + (i * LINE_HEIGHT),
                     TEXT_FONT_SIZE,
                     WHITE);
        }

        // 5. Portrait (con gestione errori migliorata)
        const std::string portraitPath = getCurrentPortrait();
        if (!portraitPath.empty())
        {
            renderPortrait(portraitPath, BOX_X + PORTRAIT_PADDING, BOX_Y + PORTRAIT_PADDING);
        }
        else
        {
            renderPlaceholderPortrait(BOX_X + PORTRAIT_PADDING, BOX_Y + PORTRAIT_PADDING,
                                      PORTRAIT_SIZE);
        }

        // 6. Indicatore di continuazione (lampeggiante)
        if (isTextComplete_ && static_cast<int>(GetTime() * 2) % 2 == 0)
        {
            const char* indicator = hasMoreLines() ? "▼" : "■";
            DrawText(indicator, BOX_X + boxWidth - 40, BOX_Y + boxHeight - 40, 28, WHITE);
        }

        // 7. Istruzioni
        const int instructionsY = BOX_Y + boxHeight - 30;
        if (isTextComplete_)
        {
            const char* spaceText = hasMoreLines() ? "SPACE - Continua" : "SPACE - Chiudi";
            DrawText(spaceText, BOX_X + TEXT_START_X, instructionsY, 16, LIGHTGRAY);
        }
        else
        {
            DrawText("SPACE - Velocizza", BOX_X + TEXT_START_X, instructionsY, 16, LIGHTGRAY);
        }

        DrawText("ESC - Salta", BOX_X + boxWidth - 120, instructionsY, 16, GRAY);
    }
}
