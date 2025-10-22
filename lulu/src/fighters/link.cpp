#include "fighters/link.hpp"
#include "arena.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

namespace lulu
{
    Link::Link(Vec2<float> pos, const std::string& configPath) : Fighter(pos, configPath)
    {
        // Leggi il JSON
        std::ifstream f(configPath);
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
        originalSize_ = size_;

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

        // Setup animazioni attacco
        const auto& attack = animations["attack"];
        std::vector<std::string> attackUp = attack["up"];
        std::vector<std::string> attackDown = attack["down"];
        std::vector<std::string> attackLeft = attack["left"];
        std::vector<std::string> attackRight = attack["right"];

        // Aggiungi animazioni attacco per tutte le direzioni
        movement_.addAnimation(S_ATTACK, D_UP, attackUp);
        movement_.addAnimation(S_ATTACK, D_UPLEFT, attackUp);
        movement_.addAnimation(S_ATTACK, D_UPRIGHT, attackUp);
        movement_.addAnimation(S_ATTACK, D_DOWN, attackDown);
        movement_.addAnimation(S_ATTACK, D_DOWNLEFT, attackDown);
        movement_.addAnimation(S_ATTACK, D_DOWNRIGHT, attackDown);
        movement_.addAnimation(S_ATTACK, D_LEFT, attackLeft);
        movement_.addAnimation(S_ATTACK, D_RIGHT, attackRight);

        // Inizializza animazione
        movement_.set(S_STILL, D_UP);
        sprite_ = movement_.nextSprite();
    }

    State Link::updatedState() const
    {
        // Mantieni lo stato corrente se già in attacco o ferito
        if (const State state = movement_.currentState(); state == S_ATTACK || state == S_HURT)
            return state;

        // Inizia attacco se premuto SPAZIO (o già in corso)
        if (arena_->isKeyJustPressed(K_SPACE) || isAttacking_)
            return S_ATTACK;

        // Movimento se c'è input direzionale
        if (updatedDirection() != D_NONE)
            return S_MOVING;

        return S_STILL;
    }

    Direction Link::updatedDirection() const
    {
        bool w = false, a = false, s = false, d = false;

        // Check which directional keys are currently pressed
        for (const Key key : arena_->currInputs())
        {
            switch (key)
            {
            case K_W:
            case K_UP:
                w = true;
                break;
            case K_A:
            case K_LEFT:
                a = true;
                break;
            case K_S:
            case K_DOWN:
                s = true;
                break;
            case K_D:
            case K_RIGHT:
                d = true;
                break;
            default:
                break;
            }
        }

        // Resolve conflicting inputs (opposite directions cancel out)
        if (a && d)
            a = d = false;
        if (w && s)
            w = s = false;

        // Determine final direction (diagonal directions have priority)
        if (w && a)
            return D_UPLEFT;
        if (w && d)
            return D_UPRIGHT;
        if (s && a)
            return D_DOWNLEFT;
        if (s && d)
            return D_DOWNRIGHT;
        if (w)
            return D_UP;
        if (s)
            return D_DOWN;
        if (a)
            return D_LEFT;
        if (d)
            return D_RIGHT;

        return D_NONE;
    }

    Vec2<float> Link::calculateMovement(const Direction dir) const
    {
        Vec2<float> movement{};
        const auto diagonal = speed_.diagonal().value();

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
            movement = {-diagonal.x, -diagonal.y};
            break;
        case D_UPRIGHT:
            movement = {diagonal.x, -diagonal.y};
            break;
        case D_DOWNLEFT:
            movement = {-diagonal.x, diagonal.y};
            break;
        case D_DOWNRIGHT:
            movement = {diagonal.x, diagonal.y};
            break;
        case D_NONE:
        default:
            break;
        }

        return movement;
    }

    void Link::adjustPositionForSize(const Vec2<float>& sizeDifference)
    {
        // Aggiusta la posizione in base alla direzione per mantenere l'allineamento
        switch (movement_.currentDirection())
        {
        case D_UP:
        case D_UPLEFT:
        case D_UPRIGHT:
            // Per attacchi verso l'alto, sposta Link verso l'alto
            pos_.y -= sizeDifference.y;
            break;

        case D_LEFT:
        case D_DOWNLEFT:
            // Per attacchi verso sinistra, sposta Link verso sinistra
            pos_.x -= sizeDifference.x;
            break;

        // DOWN e RIGHT non richiedono aggiustamenti (sprite espande verso basso/destra)
        default:
            break;
        }
    }

    void Link::setupAttack()
    {
        isAttacking_ = true;
        attackFrame_ = 0;
        originalSize_ = size_;
        movement_.set(S_ATTACK, movement_.currentDirection());
    }

    void Link::performAttack()
    {
        // Aggiorna il frame dell'attacco
        attackFrame_++;

        // Non avanzare oltre l'ultimo frame
        if (movement_.currentFrame() >= ATTACK_DURATION - 1)
            return;

        // TODO: Nel frame di danno, controlla collisioni con nemici e infliggi danno
        if (attackFrame_ == DAMAGE_FRAME)
        {
            // Qui andrà la logica per infliggere danno ai nemici colpiti
            // Per ora è solo un placeholder
        }

        // Avanza all'animazione successiva
        sprite_ = movement_.nextSprite();

        // Aggiorna le dimensioni in base alla nuova sprite
        const Vec2<float> oldSize = size_;
        size_ = AnimationHandler::getSpriteDimension(sprite_).value_or(originalSize_);

        // Aggiusta la posizione se le dimensioni sono cambiate
        const Vec2<float> sizeDiff = size_ - oldSize;
        adjustPositionForSize(sizeDiff);
    }

    void Link::endAttack()
    {
        // Reset dei flag di attacco
        isAttacking_ = false;
        attackFrame_ = 0;

        // Torna allo stato di movimento
        movement_.set(S_MOVING, movement_.currentDirection());
        sprite_ = movement_.nextSprite();

        // Ripristina le dimensioni originali
        const Vec2<float> oldSize = size_;
        size_ = AnimationHandler::getSpriteDimension(sprite_).value_or(originalSize_);

        // Aggiusta la posizione finale
        const Vec2<float> sizeDiff = size_ - oldSize;
        adjustPositionForSize(sizeDiff);
    }

    void Link::handleCollision(Collision collision)
    {
        // Durante l'attacco, Link ignora collisioni con oggetti statici
        // ma continua a collidere con entità mobili (altri Fighter)
        Actor* other = collision.target;

        if (dynamic_cast<Movable*>(other) == nullptr && isAttacking_)
        {
            // Ignora collisioni con oggetti statici durante attacco
            return;
        }

        // Gestione normale delle collisioni
        Actor::handleCollision(collision);
    }

    void Link::move()
    {
        const State newState = updatedState();
        const Direction newDirection = updatedDirection();
        static uint8_t animationSwitch = 0;

        if (newState == S_STILL)
        {
            // Transizione a stato fermo
            if (movement_.currentState() != S_STILL)
                movement_.set(S_STILL, movement_.currentDirection());
        }
        else if (newState == S_MOVING)
        {
            // Gestione movimento
            if (newDirection != D_NONE)
            {
                pos_ += calculateMovement(newDirection);

                // Cambio direzione richiede reset animazione
                if (movement_.currentDirection() != newDirection)
                {
                    movement_.set(S_MOVING, newDirection);
                    sprite_ = movement_.nextSprite();
                    animationSwitch = 0;
                }

                // Update sprite ogni 4 frame per animazione fluida
                if (animationSwitch++ % 4 == 0)
                    sprite_ = movement_.nextSprite();
            }
        }
        else if (newState == S_ATTACK)
        {
            // Gestione attacco
            if (!isAttacking_)
            {
                setupAttack();
            }

            performAttack();

            // Termina attacco quando raggiunge la durata massima
            if (attackFrame_ >= ATTACK_DURATION)
            {
                endAttack();
            }
        }
    }
} // namespace lulu
