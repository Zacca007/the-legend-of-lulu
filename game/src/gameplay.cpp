#include "gameplay.hpp"
#include <nlohmann/json.hpp>
#include "game.hpp"

namespace game
{
    Gameplay::Gameplay(Game* game, const std::string& configPath)
        : GameScene(game, configPath), arena_(configPath)
    {
        arena_.spawn(std::make_unique<lulu::Link>(lulu::Vec2{375, 400}.convert<float>()));

        // Carica le texture dei cuori una volta sola
        heartFull_ = LoadTexture("assets/ui/hearts/full_heart.png");
        heartHalf_ = LoadTexture("assets/ui/hearts/half_heart.png");
        heartEmpty_ = LoadTexture("assets/ui/hearts/empty_heart.png");
    }

    Gameplay::~Gameplay()
    {
        for (const auto& texture : textureCache_ | std::views::values)
            UnloadTexture(texture);

        // Scarica le texture dei cuori
        UnloadTexture(heartFull_);
        UnloadTexture(heartHalf_);
        UnloadTexture(heartEmpty_);
    }

    Texture2D Gameplay::getTexture(const std::string& path)
    {
        // Usa emplace per evitare lookup doppio
        auto [it, inserted] = textureCache_.try_emplace(path, Texture2D{});
        if (inserted)
        {
            it->second = LoadTexture(path.c_str());
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
            handleDialogueInput(deltaTime);
        }
        else
        {
            handleGameplayInput();
        }
    }

    void Gameplay::handleDialogueInput(float deltaTime)
    {
        dialogueManager_.update(deltaTime);

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

    void Gameplay::handleGameplayInput()
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

    void Gameplay::startDialogue(const lulu::NPC* npc)
    {
        const auto dialogueLines = npc->loadDialogue();
        dialogueManager_.startDialogue(npc, dialogueLines);
    }

    void Gameplay::renderHearts(const float currentHp) const
    {
        constexpr float MAX_HP = 6.0f;

        // Calcola il numero totale di cuori da mostrare (basato su maxHp)
        constexpr int totalHearts = static_cast<int>(MAX_HP / 2.0f);

        // Dimensione di un cuore (assumo che le texture siano quadrate)
        const int heartSize = heartFull_.width;

        for (int i = 0; i < totalHearts; ++i)
        {
            // Posizione iniziale dei cuori (angolo in alto a sinistra)
            constexpr int HEART_START_X = 100;
            constexpr int HEART_START_Y = 50;
            constexpr int HEART_SPACING = 5;

            // Calcola quanta HP rappresenta questo cuore (2 HP per cuore)
            const float heartMinHp = i * 2.0f;
            const float heartMaxHp = heartMinHp + 2.0f;

            // Posizione X del cuore corrente
            const int heartX = HEART_START_X + i * (heartSize + HEART_SPACING);

            // Determina quale texture usare
            Texture2D heartTexture;

            if (currentHp >= heartMaxHp)
            {
                // Cuore pieno: HP corrente copre completamente questo cuore
                heartTexture = heartFull_;
            }
            else if (currentHp > heartMinHp)
            {
                // Cuore mezzo: HP corrente è tra min e max di questo cuore
                // (significa che ha almeno 1 HP ma meno di 2)
                heartTexture = heartHalf_;
            }
            else
            {
                // Cuore vuoto: HP corrente non raggiunge questo cuore
                heartTexture = heartEmpty_;
            }

            // Disegna il cuore
            DrawTexture(heartTexture, heartX, HEART_START_Y, WHITE);
        }
    }

    void Gameplay::render()
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background_, 0, 0, WHITE);

        renderActors();

        // Renderizza i cuori (HP di Link)
        if (lulu::Link* pLink = findLink())
        {
            renderHearts(pLink->hp());
        }

        if (dialogueManager_.isActive())
        {
            dialogueManager_.render();
        }

        EndDrawing();
    }

    void Gameplay::renderActors()
    {
        for (const auto& actor : arena_.actors())
        {
            if (const std::string& sprite = actor->sprite(); !sprite.empty())
            {
                const Texture2D& texture = getTexture(sprite);
                auto [x, y] = actor->pos().convert<int>();
                DrawTexture(texture, x, y, WHITE);
            }
        }
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
        const auto& collisions = arena_.collisions();
        auto it = collisions.find(link);
        if (it == collisions.end()) return std::nullopt;

        for (const auto& [target, direction] : it->second)
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
        const auto& collisions = arena_.collisions();
        auto it = collisions.find(link);
        if (it == collisions.end()) return std::nullopt;

        for (const auto& [target, direction] : it->second)
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