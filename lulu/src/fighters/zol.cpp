#include "fighters/zol.hpp"
#include <nlohmann/json.hpp>
#include <random>

namespace lulu {
    Zol::Zol(Vec2<float> pos, const std::string& config) : Fighter(pos, config)
    {
        // Leggi il JSON
        std::ifstream f(config);
        nlohmann::json j;
        f >> j;

        // Parse dei dati base
        const auto& actorData = j["actor"];
        const auto& movableData = j["movable"];
        const auto& fighterData = j["fighter"];

        // Inizializza sprite base
        sprite_ = actorData["sprite"];

        // Inizializza size e salva come originale
        size_ = Vec2{actorData["size"]["width"].get<float>(), actorData["size"]["height"].get<float>()};

        // Inizializza velocità
        speed_ = Vec2{movableData["speed"]["x"].get<float>(), movableData["speed"]["y"].get<float>()};

        // Inizializza hp e damage
        hp_ = fighterData["hp"].get<float>();
        damage_ = fighterData["damage"].get<float>();

        // Abilita animazioni
        movement_.enabled_ = movableData["enableAnimation"].get<bool>();

        const auto& animations = j["animations"];

        // Setup animazioni movimento
        const auto& movement = animations["movement"];
        std::vector<std::string> up = movement["up"];
        std::vector<std::string> down = movement["down"];
        std::vector<std::string> left = movement["left"];
        std::vector<std::string> right = movement["right"];

        std::vector<std::string> death = animations["death"];
        movement_.addAnimation(S_DEAD, D_NONE, death);

        // Aggiungi animazioni movimento per tutte le direzioni
        movement_.addAnimation(S_MOVING, D_UP, up);
        movement_.addAnimation(S_MOVING, D_UPLEFT, up);
        movement_.addAnimation(S_MOVING, D_UPRIGHT, up);
        movement_.addAnimation(S_MOVING, D_DOWN, down);
        movement_.addAnimation(S_MOVING, D_DOWNLEFT, down);
        movement_.addAnimation(S_MOVING, D_DOWNRIGHT, down);
        movement_.addAnimation(S_MOVING, D_LEFT, left);
        movement_.addAnimation(S_MOVING, D_RIGHT, right);

        // Aggiungi animazioni still (uguali a quelle di movimento)
        movement_.addAnimation(S_STILL, D_UP, up);
        movement_.addAnimation(S_STILL, D_UPLEFT, up);
        movement_.addAnimation(S_STILL, D_UPRIGHT, up);
        movement_.addAnimation(S_STILL, D_DOWN, down);
        movement_.addAnimation(S_STILL, D_DOWNLEFT, down);
        movement_.addAnimation(S_STILL, D_DOWNRIGHT, down);
        movement_.addAnimation(S_STILL, D_LEFT, left);
        movement_.addAnimation(S_STILL, D_RIGHT, right);

        // Inizializza con direzione casuale
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 7);

        switch (dis(gen))
        {
            case 0: currentDirection_ = D_UP; break;
            case 1: currentDirection_ = D_DOWN; break;
            case 2: currentDirection_ = D_LEFT; break;
            case 3: currentDirection_ = D_RIGHT; break;
            case 4: currentDirection_ = D_UPLEFT; break;
            case 5: currentDirection_ = D_UPRIGHT; break;
            case 6: currentDirection_ = D_DOWNLEFT; break;
            case 7: currentDirection_ = D_DOWNRIGHT; break;
            default: currentDirection_ = D_DOWN; break;
        }

        // Imposta stato iniziale
        movement_.set(S_MOVING, currentDirection_);
        sprite_ = movement_.nextSprite();
    }

    State Zol::updatedState() const
    {
        if (!isAlive())
        {
            return S_DEAD;
        }
        // Pattern: si muove per 45 frame, poi si ferma per 15 frame
        if (movementCounter_ < 45)
        {
            return S_MOVING;
        }
        return S_STILL;
    }

    Direction Zol::updatedDirection() const
    {
        // Cambia direzione ogni 30 frame circa
        if (directionCounter_ >= 30)
        {
            // Genera una direzione casuale
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> dis(0, 7);

            const int randomDir = dis(gen);
            Direction newDirection;

            switch (randomDir)
            {
            case 0: newDirection = D_UP; break;
            case 1: newDirection = D_DOWN; break;
            case 2: newDirection = D_LEFT; break;
            case 3: newDirection = D_RIGHT; break;
            case 4: newDirection = D_UPLEFT; break;
            case 5: newDirection = D_UPRIGHT; break;
            case 6: newDirection = D_DOWNLEFT; break;
            case 7: newDirection = D_DOWNRIGHT; break;
            default: newDirection = D_DOWN; break;
            }

            return newDirection;
        }

        return currentDirection_;
    }

    Vec2<float> Zol::calculateMovement(const Direction dir) const
    {
        Vec2<float> movement{};
        const auto [x, y] = speed_.diagonal().value();

        switch (dir)
        {
        case D_UP:
            movement = {0, -speed_.y};
            break;
        case D_DOWN:
            movement = {0, speed_.y};
            break;
        case D_LEFT:
            movement = {-speed_.x, 0};
            break;
        case D_RIGHT:
            movement = {speed_.x, 0};
            break;
        case D_UPLEFT:
            movement = {-x, -y};
            break;
        case D_UPRIGHT:
            movement = {x, -y};
            break;
        case D_DOWNLEFT:
            movement = {-x, y};
            break;
        case D_DOWNRIGHT:
            movement = {x, y};
            break;
        case D_NONE:
        default:
            break;
        }

        return movement;
    }

    void Zol::move()
    {
        const State newState = updatedState();
        const Direction newDirection = updatedDirection();

        // Aggiorna i contatori
        movementCounter_++;
        if (movementCounter_ >= 60) movementCounter_ = 0;

        directionCounter_++;
        if (directionCounter_ > 30)
        {
            directionCounter_ = 0;
            currentDirection_ = newDirection; // Aggiorna la direzione corrente
        }

        if (newState == S_STILL)
        {
            // Quando è fermo, mantiene l'animazione ma non si muove
            if (movement_.currentState() != S_STILL)
            {
                movement_.set(newState, currentDirection_);
                sprite_ = movement_.nextSprite();
            }

            // Anche quando è fermo, continua l'animazione lentamente
            if (animationCounter_++ % 12 == 0) // Ancora più lento quando fermo
            {
                sprite_ = movement_.nextSprite();
            }
        }
        else if (newState == S_MOVING)
        {
            // Gestione movimento
            if (newDirection != D_NONE)
            {
                // Applica il movimento
                pos_ += calculateMovement(newDirection);

                // Cambio direzione richiede reset animazione
                if (movement_.currentDirection() != newDirection)
                {
                    movement_.set(newState, newDirection);
                    sprite_ = movement_.nextSprite();
                    animationCounter_ = 0;
                }

                // Update sprite ogni 8 frame per animazione più lenta
                if (animationCounter_++ % 8 == 0)
                {
                    sprite_ = movement_.nextSprite();
                }
            }
        }
        else if (newState == S_DEAD)
        {
            static int frameCount = 0;
            if (movement_.currentState() != S_DEAD)
            {
                movement_.set(newState, D_NONE);
                return;
            }
            if (frameCount < movement_.currentAnimation().size())
            {
                sprite_ = movement_.nextSprite();
                frameCount++;
            }
            else
            {
                frameCount = 0;
                shouldDie_ = true;
            }
        }
    }

} // namespace lulu