#include "utility actors/npc.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

namespace lulu
{
    NPC::NPC(const Vec2<float>& pos, const Vec2<float>& size, const std::string& sprite,
             const std::string& dialoguePath, const std::string& name)
        : Actor(pos, size, sprite), dialoguePath_(dialoguePath), name_(name)
    {
    }

    std::vector<DialogueLine> NPC::loadDialogue() const
    {
        if (dialogueLoaded_) {
            return cachedDialogue_;
        }

        std::ifstream f(dialoguePath_);
        if (!f.is_open()) {
            return {{"Sistema", "[Errore: dialogo non trovato: " + dialoguePath_ + "]", ""}};
        }

        nlohmann::json j;
        f >> j;

        std::vector<DialogueLine> dialogueLines;

        if (j.contains("dialogue") && j["dialogue"].is_array()) {
            for (const auto& entry : j["dialogue"]) {
                if (entry.contains("speaker") && entry.contains("text") &&
                    entry["text"].is_array() && entry.contains("portrait")) {

                    std::string speaker = entry["speaker"].get<std::string>();
                    std::string portrait = entry["portrait"].get<std::string>();

                    // PER OGNI elemento nell'array text, crea una riga SEPARATA
                    // Questo è CORRETTO - ogni frase deve essere una riga separata
                    for (const auto& line : entry["text"]) {
                        if (line.is_string()) {
                            dialogueLines.push_back({speaker, line.get<std::string>(), portrait});
                        }
                    }
                    }
            }
        } else {
            dialogueLines.push_back({"Sistema", "[Errore: formato dialogo non valido]", ""});
        }

        cachedDialogue_ = dialogueLines;
        dialogueLoaded_ = true;
        return dialogueLines;
    }
}