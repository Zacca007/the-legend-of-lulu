# The Legend of Lulu 🎮

A 2D action-adventure game built in C++23 with Raylib, inspired by classic Zelda games. The project demonstrates clean architecture with a modular engine core (`lulu`) that powers the game implementation.

## What is it

The Legend of Lulu is a complete game featuring:
- **Menu system** with animated transitions
- **Room-based exploration** with configurable layouts
- **Player character (Link)** with movement and combat animations
- **JSON-driven configuration** for easy room customization

## Architecture

```
project/
├── lulu/              # Game engine core
│   ├── include/       # Engine headers
│   └── src/          # Engine implementation  
├── game/             # Game implementation
│   ├── include/      # Game headers
│   ├── src/         # Game logic
│   └── main.cpp     # Entry point
└── assets/          # Sprites, music, JSON configs
```

## How it works

### Core Engine (lulu)
- **Actor**: Base class for all game entities (position, size, sprite, collision)
- **Arena**: Game world container that manages actors and handles input
- **Vec2 template**: 2D vector with arithmetic operations
- **Animation system**: State-based sprite animations (moving, still, attack)
- **Collision detection**: AABB collision with directional response

### Game Implementation
- **Scene system**: Menu and Gameplay scenes with background/music
- **JSON configuration**: Rooms defined in JSON files with actors and doors
- **Link character**: Player with 8-direction movement and combat states
- **Resource management**: Texture caching and automatic cleanup

### Key Classes

```cpp
// Engine core
lulu::Actor          // Base entity
lulu::Arena          // World container  
lulu::Link           // Player character
lulu::AnimationHandler // Sprite management

// Game scenes
game::GameScene      // Base scene class
game::Menu           // Start screen
game::Gameplay       // Main game
```

## Room Configuration

Rooms are defined in JSON files:

```json
{
  "background": "assets/dungeon/hall.png",
  "music": "assets/dungeon/hall.mp3", 
  "inputs": [65, 68, 83, 87, 32],
  "arena": {
    "pos": {"x": 100, "y": 100},
    "size": {"width": 600, "height": 350},
    "actors": [
      {"pos": {"x": 200, "y": 200}, "size": {"width": 32, "height": 32}}
    ],
    "doors": [
      {
        "pos": {"x": 300, "y": 100}, 
        "size": {"width": 32, "height": 32},
        "spawn": {"x": 400, "y": 200},
        "destination": "assets/dungeon/configs/room2.json"
      }
    ]
  }
}
```

## Controls

- **WASD/Arrow Keys**: Move Link in 8 directions
- **Space**: Attack
- **Enter**: Confirm in menus

## Dependencies

- **C++23** compiler
- **Raylib** for graphics and audio
- **nlohmann/json** for configuration parsing

## Building

Standard C++ build with the required libraries. The game loads the menu scene first, then transitions to gameplay when you press Enter.

---

*A demonstration of modern C++ game development with clean separation between engine and game logic, JSON-driven content, and component-based architecture.*