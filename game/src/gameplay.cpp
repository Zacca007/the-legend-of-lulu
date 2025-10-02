#include "gameplay.hpp"
#include <nlohmann/json.hpp>
#include "game.hpp"

namespace game
{
    Gameplay::Gameplay(Game* game, const std::string& configPath)
        : GameScene(game, configPath), arena_(configPath)
    {
        arena_.spawn(std::make_unique<lulu::Link>(lulu::Vec2{375, 400}.convert<float>()));
    }

    Gameplay::~Gameplay()
    {
        for (const auto& texture : textureCache_ | std::views::values)
            UnloadTexture(texture);
    }

    Texture2D Gameplay::getTexture(const std::string& path)
    {
        const auto it = textureCache_.find(path);
        if (it == textureCache_.end())
        {
            textureCache_[path] = LoadTexture(path.c_str());
            return textureCache_[path];
        }
        return it->second;
    }

    void Gameplay::tick()
    {
        static double lastTime = GetTime();
        const double currentTime = GetTime();
        const auto deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        UpdateMusicStream(music_);

        if (dialogueManager_.isActive())
        {
            dialogueManager_.update(deltaTime);
            updateDialogue();
        }
        else
        {
            arena_.tick(activeInputs());

            lulu::Link* pLink = findLink();
            if (!pLink) return;

            // Controlla porta
            if (const auto doorInfo = checkDoorCollision(pLink))
            {
                changeRoom(pLink, doorInfo.value());
            }

            // Controlla NPC
            if (const auto npc = checkNpcCollision(pLink))
            {
                if (IsKeyDown(KEY_SPACE))
                {
                    startDialogue(npc.value());
                }
            }
        }
    }

    void Gameplay::startDialogue(const lulu::NPC* npc)
    {
        const auto dialogueLines = npc->loadDialogue();
        dialogueManager_.startDialogue(npc, dialogueLines);
    }

    void Gameplay::updateDialogue()
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            dialogueManager_.advance();

            if (!dialogueManager_.isActive())
            {
                dialogueManager_.reset();
            }
        }

        if (IsKeyPressed(KEY_ESCAPE))
        {
            dialogueManager_.reset();
        }
    }

    void Gameplay::render()
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background_, 0, 0, WHITE);

        for (const auto& actor : arena_.actors())
        {
            if (std::string sprite = actor->sprite(); !sprite.empty())
            {
                const Texture2D& texture = getTexture(actor->sprite());
                auto [x, y] = actor->pos().convert<int>();
                DrawTexture(texture, x, y, WHITE);
            }
        }

        // Renderizza il dialogo se attivo
        if (dialogueManager_.isActive())
        {
            dialogueManager_.render();
        }

        EndDrawing();
    }

    lulu::Link* Gameplay::findLink() const
    {
        for (const auto& actor : arena_.actors())
        {
            if (auto* link = dynamic_cast<lulu::Link*>(actor.get()))
            {
                return link;
            }
        }
        return nullptr;
    }

    std::optional<Gameplay::DoorInfo> Gameplay::checkDoorCollision(const lulu::Link* link) const
    {
        for (const auto& [target, direction] : arena_.collisions().at(link))
        {
            if (const auto* door = dynamic_cast<lulu::Door*>(target))
            {
                return DoorInfo{
                    door->destination(),
                    door->spawn(),
                    door->changeMusic()
                };
            }
        }
        return std::nullopt;
    }

    std::optional<const lulu::NPC*> Gameplay::checkNpcCollision(const lulu::Link* link) const
    {
        for (const auto& [target, direction] : arena_.collisions().at(link))
        {
            if (const auto* npc = dynamic_cast<lulu::NPC*>(target))
            {
                return npc;
            }
        }
        return std::nullopt;
    }

    void Gameplay::changeRoom(lulu::Link* link, const DoorInfo& doorInfo)
    {
        auto linkPtr = arena_.kill(link);
        if (!linkPtr) return;

        arena_ = lulu::Arena(doorInfo.destination);
        setBackground(doorInfo.destination);

        if (doorInfo.changeMusic)
        {
            setMusic(doorInfo.destination);
        }

        linkPtr->setPos(doorInfo.spawn);
        arena_.spawn(std::move(linkPtr));
    }
}
