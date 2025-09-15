#include "gameplay.hpp"
#include <nlohmann/json.hpp>

namespace game
{
Gameplay::Gameplay(Game *game, const std::string &configPath)
    : GameScene(game, configPath), arena_(configPath)
{
    arena_.spawn(std::make_unique<lulu::Link>(lulu::Vec2{375, 400}.convert<float>()));
}
 Gameplay::~Gameplay()
{
    for (const auto &texture : textureCache_ | std::views::values)
        UnloadTexture(texture);
}

Texture2D Gameplay::getTexture(const std::string &path)
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
    UpdateMusicStream(music_);
    arena_.tick(activeInputs());
    lulu::Link* pLink = nullptr;
    for (const auto &actor : arena_.actors())
    {
        if (pLink = dynamic_cast<lulu::Link *>(actor.get()); pLink != nullptr)
        {
            break;
        }
    }
    if (!pLink) return;

    std::string destination{};
    lulu::Vec2<float> spawn{};
    bool changeMusic = false;
    for (auto [target, collisionDirection] : arena_.collisions().at(pLink))
    {
        if (const auto *door = dynamic_cast<lulu::Door *>(target))
        {
            destination = door->destination();
            spawn = door->spawn();
            changeMusic = door->changeMusic();
            break;
        }
    }

    auto link = std::unique_ptr<lulu::Actor>{};
    if (!destination.empty()) link = arena_.kill(pLink);

    if (link)
    {
        arena_ = lulu::Arena(destination);
        setBackground(destination);
        if (changeMusic) setMusic(destination);
        link->setPos(spawn);
        arena_.spawn(std::move(link));
    }

}
void Gameplay::render()
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(background_, 0, 0, WHITE);

    auto [ax, ay] = arena_.pos().convert<int>();
    auto [aw, ah] = arena_.size().convert<int>();
    DrawRectangleLines(ax, ay, aw, ah, WHITE);
    for (const auto &actor : arena_.actors())
    {
        auto [ax, ay] = actor->pos().convert<int>();
        auto [aw, ah] = actor->size().convert<int>();
        DrawRectangleLines(ax, ay, aw, ah, WHITE);
    }

    for (const auto& actor : arena_.actors())
    {
        if (std::string sprite = actor->sprite(); !sprite.empty())
        {
            const Texture2D &texture = getTexture(actor->sprite());
            auto [x, y] = actor->pos().convert<int>();
            DrawTexture(texture, x, y, WHITE);
        }
    }

    EndDrawing();
}
} // namespace game