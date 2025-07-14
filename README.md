# The Legend of Lulu 🎮

A 2D action-adventure game engine built in C++ with Raylib, created as a heartfelt gift for someone special.

## Overview

The Legend of Lulu is a Zelda-inspired game featuring Link as the main character navigating through dungeon environments, battling enemies, and solving puzzles. The game is built with a modular architecture that separates game logic from rendering, making it extensible and maintainable.

## Features

### Core Systems
- **Actor-based Architecture**: Modular system where all game entities inherit from a base `Actor` class
- **Animation System**: Sophisticated sprite animation handling with state-based transitions
- **Collision Detection**: Comprehensive collision system with different collision types
- **Arena Management**: Game world container that manages actors and game state
- **Input Handling**: Flexible keyboard input system with support for multiple keys

### Game Elements
- **Player Character (Link)**: Fully animated character with movement, attack mechanics, and collision handling
- **Combat System**: Fighter base class providing health, damage, and attack mechanics
- **Trap Enemies**: Intelligent BladeTrap enemies with state-based AI (idle, charging, returning)
- **Static Objects**: Environmental obstacles and decorative elements
- **Scene Management**: Menu system and game state transitions

### Technical Features
- **PNG Sprite Support**: Direct PNG file reading for sprite dimensions
- **Music and Sound**: Integrated audio system with background music and sound effects
- **Texture Caching**: Efficient texture loading and management
- **Real-time Updates**: 30 FPS game loop with smooth animation transitions

## Architecture

```
core/
├── include/
│   ├── actor.hpp              # Base class for all game entities
│   ├── movable.hpp            # Interface for movable actors
│   ├── animationHandler.hpp   # Sprite animation management
│   ├── arena.hpp              # Game world container
│   ├── types.hpp              # Core data structures and enums
│   ├── lulu.hpp               # Main library header
│   └── actors/
│       ├── fighter.hpp        # Base class for combat entities
│       ├── link.hpp           # Player character
│       └── bladeTrap.hpp      # Trap enemy AI
├── assets/                    # Game assets (sprites, music, sounds)
└── main.cpp                   # Game entry point and scene management
```

## Class Hierarchy

```
Actor (base class)
├── Static Objects
└── Movable (interface)
    └── Fighter
        ├── Link (player)
        └── BladeTrap (enemy)
```

## Key Components

### Actor System
- **Actor**: Base class providing position, size, collision detection, and sprite handling
- **Movable**: Interface for actors that can move with animation support
- **Fighter**: Combat-capable actors with health, damage, and attack mechanics

### Animation System
- State-based animations (still, movement, attack, hurt)
- Direction-aware sprite sequences
- Smooth frame transitions
- PNG dimension reading for proper sprite handling

### Collision System
- Directional collision detection (top, bottom, left, right)
- Collision response handling
- Static vs dynamic actor differentiation

### Game Scenes
- **Menu**: Animated start screen with music
- **GameOn**: Main gameplay scene with arena, player, and enemies

## Controls

- **WASD** / **Arrow Keys**: Character movement
- **Space**: Attack / Menu selection
- **Enter**: Confirm actions

## Dependencies

- **Raylib**: Graphics, audio, and input handling
- **C++20**: Modern C++ features including ranges and concepts

## Building and Running

The project uses modern C++ features and requires:
- C++20 compatible compiler
- Raylib library
- Standard library support for ranges and concepts

## Game Design Philosophy

This game engine emphasizes:
- **Modularity**: Clean separation of concerns with reusable components
- **Extensibility**: Easy to add new actors, animations, and game mechanics
- **Performance**: Efficient collision detection and texture caching
- **Maintainability**: Well-documented code with clear inheritance hierarchies

## Future Enhancements

The architecture supports easy addition of:
- New enemy types with custom AI
- Additional player abilities and weapons
- More complex level designs
- Inventory and item systems
- Save/load functionality
- Multiplayer support

---

*Made with ❤️ for someone special*