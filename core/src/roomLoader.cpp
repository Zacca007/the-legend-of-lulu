#include "roomLoader.hpp"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

namespace lulu {

RoomConfig RoomLoader::loadRoom(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open room configuration file: " + filepath);
    }
    
    json jsonData;
    try {
        file >> jsonData;
    } catch (const json::parse_error& e) {
        throw std::runtime_error("JSON parse error in file " + filepath + ": " + e.what());
    }
    
    if (!jsonData.contains("room")) {
        throw std::runtime_error("Invalid room configuration: missing 'room' object");
    }
    
    const auto& roomJson = jsonData["room"];
    RoomConfig config;
    
    // Basic room information
    config.name = roomJson.value("name", "Unnamed Room");
    config.background = roomJson.value("background", "");
    config.music = roomJson.value("music", "");
    
    if (roomJson.contains("sounds") && roomJson["sounds"].is_array()) {
        for (const auto& sound : roomJson["sounds"]) {
            config.sounds.push_back(sound.get<std::string>());
        }
    }
    
    // Arena configuration
    if (roomJson.contains("arena")) {
        const auto& arena = roomJson["arena"];
        config.arenaPosition = parsePosition(arena["position"]);
        config.arenaSize = parsePosition(arena["size"]);
    }
    
    // Player configuration
    if (roomJson.contains("player")) {
        const auto& player = roomJson["player"];
        config.playerSpawnPosition = parsePosition(player["spawn_position"]);
        config.playerSize = parsePosition(player["size"]);
        config.playerSpeed = player.value("speed", 6.0f);
        config.playerHp = player.value("hp", 12.0f);
        config.playerDamage = player.value("damage", 1.0f);
    }
    
    // Static actors
    if (roomJson.contains("static_actors") && roomJson["static_actors"].is_array()) {
        for (const auto& actorJson : roomJson["static_actors"]) {
            config.staticActors.push_back(parseStaticActor(actorJson));
        }
    }
    
    // Enemies
    if (roomJson.contains("enemies") && roomJson["enemies"].is_array()) {
        for (const auto& enemyJson : roomJson["enemies"]) {
            config.enemies.push_back(parseEnemy(enemyJson));
        }
    }
    
    return config;
}

std::vector<std::unique_ptr<Actor>> RoomLoader::createActorsFromConfig(
    const RoomConfig& config, Arena* arena) {
    
    std::vector<std::unique_ptr<Actor>> actors;
    
    // Create static actors (walls, obstacles, etc.)
    for (const auto& actorConfig : config.staticActors) {
        if (actorConfig.type == "wall") {
            actors.push_back(std::make_unique<Actor>(
                actorConfig.position,
                actorConfig.size,
                arena,
                actorConfig.sprite
            ));
        }
        // Add more static actor types here as needed
    }
    
    // Create enemies
    for (const auto& enemyConfig : config.enemies) {
        if (enemyConfig.type == "bladeTrap") {
            // Note: This assumes BladeTrap will be properly implemented
            // For now, create a basic Actor as placeholder
            actors.push_back(std::make_unique<Actor>(
                enemyConfig.position,
                enemyConfig.size,
                arena,
                enemyConfig.sprite
            ));
        }
        // Add more enemy types here as they are implemented
    }
    
    return actors;
}

pair RoomLoader::parsePosition(const nlohmann::json& json) {
    if (!json.contains("x") || !json.contains("y")) {
        throw std::runtime_error("Invalid position object: missing x or y coordinate");
    }
    
    return {
        json["x"].get<float>(),
        json["y"].get<float>()
    };
}

RoomConfig::StaticActorConfig RoomLoader::parseStaticActor(const nlohmann::json& json) {
    RoomConfig::StaticActorConfig config;
    
    config.type = json.value("type", "wall");
    config.position = parsePosition(json["position"]);
    config.size = parsePosition(json["size"]);
    config.sprite = json.value("sprite", "");
    
    return config;
}

RoomConfig::EnemyConfig RoomLoader::parseEnemy(const nlohmann::json& json) {
    RoomConfig::EnemyConfig config;
    
    config.type = json.value("type", "enemy");
    config.position = parsePosition(json["position"]);
    config.size = parsePosition(json["size"]);
    config.speed = json.value("speed", 3.0f);
    config.damage = json.value("damage", 1.0f);
    config.sprite = json.value("sprite", "");
    
    return config;
}

} // namespace lulu