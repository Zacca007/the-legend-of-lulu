#pragma once
#include "dialogueManager.hpp"
#include <nlohmann/json.hpp>
#include <raylib.h>
#include "game.hpp"
#include "gameScene.hpp"

namespace game
{
    class Gameplay final : public GameScene
    {
        lulu::Arena arena_;
        std::unordered_map<std::string, Texture2D> textureCache_;

        DialogueManager dialogueManager_; // Sostituisce la vecchia struct

        struct DoorInfo
        {
            std::string destination;
            lulu::Vec2<float> spawn;
            bool changeMusic;
        };

        Texture2D getTexture(const std::string& path);
        lulu::Link* findLink() const;
        std::optional<DoorInfo> checkDoorCollision(const lulu::Link* link) const;
        std::optional<const lulu::NPC*> checkNpcCollision(const lulu::Link* link) const;
        void changeRoom(lulu::Link* link, const DoorInfo& doorInfo);
        void startDialogue(const lulu::NPC* npc);
        void updateDialogue();

    public:
        explicit Gameplay(Game* game, const std::string& configPath = "assets/dungeon/rooms/hall.json");
        ~Gameplay() override;

        void tick() override;
        void render() override;
    };
}
