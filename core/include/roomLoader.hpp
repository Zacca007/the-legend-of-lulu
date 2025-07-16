#pragma once
#include "lulu.hpp"
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

namespace lulu {

/**
 * @brief Configuration structure for a room loaded from JSON
 */
struct RoomConfig {
    std::string name;
    std::string background;
    std::string music;
    std::vector<std::string> sounds;
    
    pair arenaPosition;
    pair arenaSize;
    
    pair playerSpawnPosition;
    pair playerSize;
    float playerSpeed;
    float playerHp;
    float playerDamage;
    
    struct StaticActorConfig {
        std::string type;
        pair position;
        pair size;
        std::string sprite;
    };
    
    struct EnemyConfig {
        std::string type;
        pair position;
        pair size;
        float speed;
        float damage;
        std::string sprite;
    };
    
    std::vector<StaticActorConfig> staticActors;
    std::vector<EnemyConfig> enemies;
};

/**
 * @brief Utility class for loading room configurations from JSON files
 */
class RoomLoader {
public:
    /**
     * @brief Load room configuration from a JSON file
     * @param filepath Path to the JSON configuration file
     * @return RoomConfig structure with loaded data
     * @throws std::runtime_error if file cannot be loaded or parsed
     */
    static RoomConfig loadRoom(const std::string& filepath);
    
    /**
     * @brief Create actors from room configuration
     * @param config The loaded room configuration
     * @param arena Pointer to the arena where actors will be spawned
     * @return Vector of created actors (caller owns the memory)
     */
    static std::vector<std::unique_ptr<Actor>> createActorsFromConfig(
        const RoomConfig& config, Arena* arena);

private:
    static pair parsePosition(const nlohmann::json& json);
    static RoomConfig::StaticActorConfig parseStaticActor(const nlohmann::json& json);
    static RoomConfig::EnemyConfig parseEnemy(const nlohmann::json& json);
};

} // namespace lulu