# The Legend of Lulu 🎮

A modular 2D game engine built in C++ with Raylib, featuring a clean separation between core game engine (`lulu`) and game implementation (`game`).

## Overview

The Legend of Lulu consists of two main components:
- **Lulu Engine**: A lightweight, reusable 2D game engine core
- **Game Implementation**: A Zelda-inspired action-adventure game built on top of the engine

The project demonstrates modern C++ design principles with clean architecture, modular components, and JSON-based configuration system.

## Architecture Overview

```
project/
├── lulu/                      # Core game engine library
│   ├── include/
│   │   ├── actor.hpp          # Base entity class
│   │   ├── arena.hpp          # Game world container
│   │   ├── types.hpp          # Core data structures
│   │   └── lulu.hpp           # Main engine header
│   └── src/                   # Engine implementation
├── game/                      # Game-specific implementation
│   ├── include/
│   │   ├── game.hpp           # Main game controller
│   │   ├── gameScene.hpp      # Scene base class
│   │   ├── menu.hpp           # Menu scene
│   │   └── gameplay.hpp       # Gameplay scene
│   ├── src/                   # Game implementation
│   └── main.cpp               # Application entry point
└── assets/                    # Game resources
```

## Core Engine Features (Lulu)

### Actor System
- **Actor**: Base class for all game entities with position, size, and sprite handling
- **Arena**: Game world container managing actors and input state
- **Smart Memory Management**: Uses `std::unique_ptr` for automatic memory management

### Input System
- Cross-platform key mapping with custom `Key` enum
- Input state tracking (current and previous frame)
- Key press detection with "just pressed" functionality

### Utilities
- **Vec2 Template**: Generic 2D vector with arithmetic operations
- **Arena Boundaries**: Automatic actor constraint within arena bounds
- **Collision Detection**: Foundation for collision system (expandable)

## Game Implementation Features

### Scene Management
- **GameScene**: Abstract base class for all game scenes
- **Menu**: Animated start screen with pulsing text effect
- **Gameplay**: Main game scene with JSON-configured arenas
- **Scene Transitions**: Smooth switching between different game states

### Configuration System
- **JSON-Based Setup**: Scenes and arenas configured through JSON files
- **Asset Management**: Centralized loading of textures and audio
- **Flexible Input Mapping**: Scene-specific input configurations

### Audio & Graphics
- **Raylib Integration**: Modern graphics and audio through Raylib
- **Music Streaming**: Background music with proper resource management
- **Texture Loading**: Efficient texture handling with automatic cleanup

## JSON Configuration Example

```json
{
  "background": "assets/dungeon/hall.png",
  "music": "assets/dungeon/hall.mp3",
  "inputs": [65, 68, 83, 87, 32],
  "arena": {
    "pos": {"x": 100, "y": 100},
    "size": {"width": 600, "height": 350},
    "actors": [
      {
        "pos": {"x": 200, "y": 200},
        "size": {"width": 32, "height": 32}
      }
    ]
  }
}
```

## Class Hierarchy

```
Engine (Lulu):
Actor (base class)
└── [Extensible for specific actor types]

Arena (world container)
└── manages std::vector<std::unique_ptr<Actor>>

Game Implementation:
Game (main controller)
└── GameScene (abstract base)
    ├── Menu (start screen)
    └── Gameplay (main game)
```

## Key Design Principles

### Memory Safety
- **RAII**: Automatic resource management for textures and audio
- **Smart Pointers**: `std::unique_ptr` for actor ownership
- **Exception Safety**: Proper resource cleanup in destructors

### Modularity
- **Engine/Game Separation**: Core engine reusable for different games
- **Scene System**: Easily extensible scene management
- **JSON Configuration**: Game content separated from code

### Modern C++
- **C++20 Features**: Modern language constructs and standard library
- **Templates**: Generic Vec2 for different numeric types
- **Move Semantics**: Efficient transfer of ownership

## Controls

- **WASD** / **Arrow Keys**: Movement (configurable)
- **Space**: Action/Confirm (configurable)
- **Enter**: Menu selection (configurable)

## Dependencies

- **Raylib**: Graphics, audio, and input handling
- **nlohmann/json**: JSON parsing for configuration files
- **C++20**: Modern C++ standard library features

## Building and Running

Requirements:
- C++20 compatible compiler
- Raylib library
- nlohmann/json library

The modular architecture allows for:
```cpp
// Using the engine in your own game
#include "lulu.hpp"
lulu::Arena gameWorld{{0, 0}, {800, 600}};
auto player = std::make_unique<lulu::Actor>(position, size);
gameWorld.spawn(std::move(player));
```

## Extensibility

The engine is designed for easy extension:

### Adding New Scenes
```cpp
class NewScene : public GameScene {
    // Implement tick() and render()
};
```

### Adding New Actor Types
```cpp
class Enemy : public lulu::Actor {
    // Add custom behavior
};
```

### JSON-Driven Content
- Add new arena layouts without recompiling
- Configure different input schemes per scene
- Easy asset swapping and management

## Future Enhancements

The architecture supports:
- **Enhanced Actor System**: Movable actors, fighters, AI behaviors
- **Animation System**: Sprite-based animations with state management
- **Physics Integration**: Collision detection and response
- **Save System**: JSON-based game state persistence
- **Scripting Support**: Lua or Python integration for gameplay logic
- **Multiplayer**: Network-based multiplayer support

---

## Technical Notes

The project demonstrates several advanced C++ concepts:
- Template metaprogramming with Vec2
- RAII and smart pointer usage
- Modern exception handling
- JSON integration with type safety
- Cross-platform input abstraction
- Modular library design