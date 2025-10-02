#include "dialogueManager.hpp"
#include <raylib.h>
#include <iostream>

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

    void DialogueManager::render() const
    {
        if (!isActive_) return;

        // POSIZIONE E DIMENSIONI FISSE
        constexpr int boxX = 100;
        constexpr int boxY = 300;
        const int boxWidth = dialogueBoxTexture_.width;
        const int boxHeight = dialogueBoxTexture_.height;

        // 1) DIBG UN RETTANGOLO BLU TRASPARENTE SOTTO IL BOX
        DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{30, 60, 120, 180}); // Blu trasparente

        // 2) POI DISEGNA IL PNG DEL BOX SOPRA (con trasparenza naturale del PNG)
        DrawTexture(dialogueBoxTexture_, boxX, boxY, WHITE);

        // 3) TESTO PIÙ GRANDE e WRAPPING AUTOMATICO
        const std::string currentText = getCurrentText();
        const std::string speaker = getCurrentSpeaker();

        // Font più grande
        constexpr int fontSize = 22;
        constexpr int maxLineWidth = 450; // Larghezza massima per stare dentro al box

        // 3.1) Disegna il nome del personaggio (testo grande)
        if (!speaker.empty())
        {
            DrawText(speaker.c_str(), boxX + 150, boxY + 15, 24, WHITE);
        }

        // 3.2) Dividi il testo in righe con wrapping
        std::vector<std::string> lines;
        std::string currentLine;

        for (char c : currentText)
        {
            currentLine += c;
            // Misura la larghezza del testo
            int textWidth = MeasureText(currentLine.c_str(), fontSize);

            if (textWidth >= maxLineWidth)
            {
                // Trova l'ultimo spazio per spezzare in modo pulito
                size_t lastSpace = currentLine.find_last_of(' ');
                if (lastSpace != std::string::npos && lastSpace > 0)
                {
                    lines.push_back(currentLine.substr(0, lastSpace));
                    currentLine = currentLine.substr(lastSpace + 1);
                }
                else
                {
                    // Se non ci sono spazi, spezza comunque
                    lines.push_back(currentLine);
                    currentLine.clear();
                }
            }
        }

        if (!currentLine.empty())
        {
            lines.push_back(currentLine);
        }

        // 3.3) Disegna le righe (massimo 3 righe)
        constexpr int startY = boxY + 60;

        for (size_t i = 0; i < lines.size() && i < 3; i++)
        {
            constexpr int lineHeight = 30;
            DrawText(lines[i].c_str(), boxX + 150, startY + (i * lineHeight), fontSize, WHITE);
        }

        // 4) GESTIONE RITRATTI - VERSIONE SEMPLICE
        std::string portraitPath = getCurrentPortrait();
        if (!portraitPath.empty())
        {
            // DEBUG
            std::cout << "Tentativo di caricare ritratto: " << portraitPath << std::endl;

            Texture2D portrait = LoadTexture(portraitPath.c_str());
            if (portrait.id != 0)
            {
                std::cout << "SUCCESSO - Ritratto caricato, ID: " << portrait.id << std::endl;
                DrawTexture(portrait, boxX + 20, boxY + 20, WHITE);
                // NON UNLOADARE QUI - lascia in memoria per questo frame
            }
            else
            {
                std::cout << "FALLIMENTO - Ritratto non caricato" << std::endl;
                // Placeholder
                DrawRectangle(boxX + 20, boxY + 20, 100, 100, Color{50, 50, 80, 200});
                DrawRectangleLines(boxX + 20, boxY + 20, 100, 100, BLUE);
                DrawText("?", boxX + 60, boxY + 60, 30, WHITE);
                DrawText("No img", boxX + 30, boxY + 130, 12, LIGHTGRAY);
            }

            // Indicatore di continuazione (lampeggiante)
            if (isTextComplete_)
            {
                if (static_cast<int>(GetTime() * 2) % 2 == 0)
                {
                    const char* indicator = hasMoreLines() ? "▼" : "■";
                    DrawText(indicator, boxX + boxWidth - 40, boxY + boxHeight - 40, 28, WHITE);
                }
            }

            // Istruzioni con testo più leggibile
            const int instructionsY = boxY + boxHeight - 30;
            if (isTextComplete_)
            {
                const char* spaceText = hasMoreLines() ? "SPACE - Continua" : "SPACE - Chiudi";
                DrawText(spaceText, boxX + 150, instructionsY, 16, LIGHTGRAY);
            }
            else
            {
                DrawText("SPACE - Velocizza", boxX + 150, instructionsY, 16, LIGHTGRAY);
            }

            DrawText("ESC - Salta", boxX + boxWidth - 120, instructionsY, 16, GRAY);
        }
    } // namespace game
}
