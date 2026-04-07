# 🗺️ Adventure World — 2-Player Co-op Console Puzzle Platformer
> A fully-featured, two-player cooperative puzzle platformer running entirely in the Windows console, built in **C++17** from scratch — no game engine, no third-party libraries.
---
## 📋 Table of Contents
- [Overview](#overview)
- [Gameplay Demo](#gameplay-demo)
- [Features](#features)
- [Architecture](#architecture)
- [Design Patterns](#design-patterns)
- [Game Objects](#game-objects)
- [Game Modes](#game-modes)
- [Controls](#controls)
- [How to Build & Run](#how-to-build--run)
- [Level Format](#level-format)
- [Project Structure](#project-structure)
---
## Overview
**Adventure World** is a two-player cooperative puzzle game where both players share a single keyboard and must work together to escape a 6-level facility. Each level is a hand-crafted ASCII board filled with puzzles, enemies, traps, and mechanics that require coordination to overcome.
The project was developed as a final assignment in an advanced Object-Oriented Programming course and demonstrates mastery of C++ OOP principles, clean architecture, and software design patterns.
---
## Gameplay Demo
```
################################################################################
##  ___ _  _ ___ _____ ___   _ _  ___  ___ ____  ___  __  __ ____            ##
## |_ _| \| / __|_   _| _ \ | | |/ __||   _| _ \/   \| | | |/ __|            ##
##  | || .` \__ \ | | |   / | | | (__  | | |   / (_) | |_| |\__ \            ##
## |___|_|\_|___/ |_| |_|_\_\___/\___| |_| |_|_\\___/ \___/ |___/            ##
################################################################################
  PLAYER 1 ($): W A X D  │  PLAYER 2 (&): I J M L  │  [ESC] Pause
################################################################################
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
W  $  K!             W         %         W           !K  &     W
W      .    .        W     ^       ^     W      .    .         W
WWWWWWWWWWWWWWW?WWWWWW     ^       ^     WWWWWW?WWWWWWWWWWWWWWW
W                    W     WWWWW WWWWW   W                     W
W    @    K    @     W         %         W    @    K    @      W
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
  P1: ❤❤❤  Score:0  │  P2: ❤❤❤  Score:0  │  Level 1  │  Time: 0:00
```
---
## Features
| Feature | Description |
|---|---|
| 🎮 **2-Player Co-op** | Both players on the same keyboard, sharing a level with independent lives |
| 🧩 **6 Hand-crafted Levels** | Loaded from editable `.screen` text files |
| 🧠 **Riddle System** | Text-based riddles that gate progress, loaded from an external file |
| 💾 **Save & Load** | Full game state persistence (position, score, lives, inventory, level) |
| 🔁 **Record & Replay** | Save keypress sequences and replay them deterministically (used for testing) |
| 🤫 **Silent Mode** | Headless automated replay for CI-style regression testing |
| 👾 **Enemy AI** | Enemies patrol and respond to player positions |
| 💣 **Bomb System** | Timed explosions with a blast radius of 3 cells |
| 🌱 **Spring Mechanics** | Compress springs then launch with momentum transfer |
| 🔦 **Fog / Torch** | Dark rooms that only reveal content when a torch is carried |
| ⚡ **Momentum & Physics** | Players carry velocity after spring launches; energy transfers between players on collision |
| 🎨 **16-Color Console** | Each object type has its own color via the Windows Console API |
| 🖥️ **Full UI** | Animated menus, level-transition screens, game-over screen — all loaded from ASCII art text files |
---
## Architecture
The game is built around a clean **Manager-based architecture**. Each subsystem is fully encapsulated in its own class, and the core game loop delegates to these managers.
```
main()
  └── GameFactory::createGame()       ← chooses mode from CLI args
        └── Game (abstract base)
              ├── GameSimple          ← interactive play
              ├── GameSave            ← interactive + record keystrokes
              └── GameLoad            ← replay / silent CI mode
Game (base tick loop)
  ├── ScreenGameManager   ← owns all 6 level Screens, handles transitions
  ├── InputManager        ← delegates to InputStrategy (keyboard/replay)
  ├── PhysicsManager      ← collision detection, momentum, spring launch
  ├── EntityManager       ← enemy movement, bomb timers & explosions
  ├── InteractionManager  ← player↔object and player↔player collisions
  ├── RiddleManager       ← loads and evaluates riddles from file
  ├── SaveManager         ← serializes/deserializes full game state
  ├── RecordManager       ← records and replays input sequences
  ├── MenuManager         ← main menu, instructions, transitions
  ├── LevelManager        ← level progression logic
  ├── ObjectManager       ← per-screen object registry
  ├── ColorManager        ← maps ObjectType → console Color
  ├── BoardRenderer       ← low-level console cell drawing
  ├── LegendManager       ← HUD / status bar rendering
  └── ErrorManager        ← centralized error logging
```
### Game Object Hierarchy
```
BaseGameObject  (virtual handleCollision, virtual update)
  ├── WallObject
  ├── DoorObject      ← requires key; cooperative puzzle
  ├── KeyObject
  ├── BombObject      ← timed, multi-cell explosion
  ├── EnemyObject     ← AI movement, killed by sword or bomb
  ├── TorchObject     ← reveals fog rooms
  ├── RiddleObject    ← shows riddle, validates answer
  ├── SpringObject    ← compressible launch pad
  ├── TeleportObject  ← warps player to paired teleport
  ├── SwitchObject    ← toggles linked mechanisms
  ├── SwordObject     ← enables enemy kills
  ├── MineObject      ← instant kill trap
  ├── ExtraLifeObject
  ├── ObstacleObject  ← pushable crate
  └── ExitObject      ← level exit (locked until conditions met)
```
---
## Design Patterns
| Pattern | Where Used |
|---|---|
| **Singleton** | `Game::instance` — single authoritative game state |
| **Factory** | `GameFactory::createGame()` — selects `GameSimple` / `GameSave` / `GameLoad` from CLI args |
| **Template Method** | `Game::baseRunTick()` — fixed algorithm with virtual hooks (`onTickBegin`, `onTickEnd`, `shouldRenderUI`, etc.) |
| **Strategy** | `InputStrategy` — `KeyboardStrategy` / `SaveStrategy` / `LoadStrategy` swap at runtime |
| **Polymorphism** | `BaseGameObject::handleCollision()` — each object type defines its own reaction |
| **Manager Pattern** | All subsystems encapsulated as stateless or minimal-state manager classes |
---
## Game Objects
| Symbol | Object | Behavior |
|---|---|---|
| `$` | Player 1 | Controlled with W/A/X/D |
| `&` | Player 2 | Controlled with I/J/M/L |
| `W` | Wall | Impassable |
| `D` / `0` | Door | Opens when a player uses a key |
| `K` | Key | Collectible; opens doors |
| `@` | Bomb | Throw with E/O; explodes after delay, blast radius 3 |
| `%` | Enemy | Moves autonomously; kills on contact |
| `\|` | Sword | Allows killing enemies; lost on level exit |
| `!` | Torch | Lights dark (fog) rooms; lost on level exit |
| `?` | Riddle | Blocks passage until the correct answer is typed |
| `#` | Spring | Compress by moving into it; release for momentum launch |
| `^` | Teleport | Warps player to the paired teleport tile |
| `/` `\` | Switch | Toggles linked mechanisms (on/off) |
| `*` | Crate | Pushable obstacle |
| `.` | Mine | Instant-kill trap |
| `+` | Extra Life | Grants one additional life |
| `X` / `x` | Exit | Closed / open; both players must reach it to advance |
| `F` | Fog | Darkness; revealed only while carrying a torch |
---
## Game Modes
```
MyGame.exe                         # Interactive play
MyGame.exe -save  steps.txt        # Play and record every keystroke
MyGame.exe -load  steps.txt        # Replay recorded session visually
MyGame.exe -silent steps.txt       # Headless replay for automated testing
```
| Mode | Description |
|---|---|
| **Simple** | Normal interactive play |
| **Save** | Same as simple, but every key press is recorded to a `.txt` file |
| **Load** | Replays a previously saved session on screen |
| **Silent** | Headless (no rendering) deterministic replay; outputs a result file for diff-based testing |
---
## Controls
| Action | Player 1 | Player 2 |
|---|---|---|
| Move Up | `W` | `I` |
| Move Down | `X` | `M` |
| Move Left | `A` | `J` |
| Move Right | `D` | `L` |
| Throw item | `E` | `O` |
| Pause / Menu | `ESC` | `ESC` |
---
## How to Build & Run
### Requirements
- **Windows** (uses Win32 Console API)
- **Visual Studio 2019+** (or any C++17-compatible MSVC toolchain)
### Build with Visual Studio
1. Open `MyGame.sln`
2. Select **Release** or **Debug** configuration
3. Press **Ctrl+Shift+B** to build
4. Run with **F5** or from the terminal
### Build with MSVC from command line
```bat
cl /std:c++17 /EHsc /Fe:MyGame.exe *.cpp
MyGame.exe
```
### File Dependencies
The executable expects these files in the **same directory**:
```
adv-world_01.screen  …  adv-world_06.screen   ← level layouts
Riddles.txt                                    ← riddle questions & answers
theMainMenu.txt                                ← main menu ASCII art
gameUserInstructions.txt                       ← in-game help screen
FinishGame.txt  GameOver.txt  ExitGame.txt     ← end-screen art
LevelUpDisplay.txt  LevelDownDisplay.txt       ← transition screens
GAME_SAVED_SUCCESSFULLY.txt                    ← save confirmation
```
---
## Level Format
Levels are plain-text `.screen` files (80×25 characters). Any text editor can be used to design new levels. The first line is a flags line; the remaining 24 lines are the board.
```
LF                          ← Level Flags line
WWWWWWWWWWWWWWWWWWWWW...    ← Row 1 (walls)
W  $  K!          W...      ← Row 2 (Player 1 spawn, key, torch, ...)
...
WWWWWWWWWWWWWWWWWWWWW...    ← Row 24 (walls)
```
To add a new level: create `adv-world_07.screen`, register it in `Constants.h`, and add it to the load sequence in `ScreenGameManager`.
---
## Project Structure
```
.
├── main.cpp                  Entry point
├── Constants.h               All enums, symbol constants, file name constants
│
├── Game.{h,cpp}              Abstract base game loop (Template Method)
├── GameSimple.{h,cpp}        Interactive mode
├── GameSave.{h,cpp}          Record mode
├── GameLoad.{h,cpp}          Replay / silent mode
├── GameFactory.h             CLI-arg → game mode factory
│
├── Player.{h,cpp}            Player state, inventory, spring, momentum
├── BaseGameObject.h          Polymorphic game object base class
├── *Object.h                 Concrete object types (Door, Key, Bomb, …)
│
├── PhysicsManager.{h,cpp}    Movement, collision, momentum
├── InteractionManager.{h,cpp}Player↔object and player↔player logic
├── EntityManager.{h,cpp}     Enemy AI, bomb timers
├── InputManager.{h,cpp}      Key dispatch via InputStrategy
├── InputStrategy.h           Abstract input interface
├── InputStrategies.h         Keyboard / Save / Load concrete strategies
│
├── Screen.{h,cpp}            Single level board + object registry
├── ScreenGameManager.{h,cpp} Level loading, transitions, current screen
├── RoomBoard.{h,cpp}         Raw board data management
├── BoardRenderer.{h,cpp}     Console cell rendering
│
├── SaveManager.{h,cpp}       Full state save/load (file I/O)
├── RecordManager.{h,cpp}     Keystroke record/replay
├── RiddleManager.{h,cpp}     Riddle loading and interaction
├── MenuManager.{h,cpp}       Menu, instructions, transition screens
│
├── ColorManager.{h,cpp}      ObjectType → Color mapping
├── LegendManager.{h,cpp}     HUD rendering
├── ErrorManager.h            Error logging
├── Utils.{h,cpp}             Console helpers (cls, sleep, cursor)
├── Move.{h,cpp}              Direction → Point delta
├── Point.h                   2D coordinate struct
│
├── adv-world_0[1-6].screen   Level definition files
├── Riddles.txt               Riddle questions and answers
└── *.txt                     UI / menu ASCII art files
```
---
## Technical Highlights
- **Zero external dependencies** — only the C++17 standard library and Win32 Console API
- **Deterministic replay** — the game loop is tick-based with a fixed seed, enabling byte-exact reproducibility for automated testing
- **Data-driven level design** — all levels, riddles, and UI screens are external files; no recompilation needed to edit content
- **Extensible object system** — adding a new game object requires only a new class deriving from `BaseGameObject` and a new entry in the `ObjectType` enum
- **Clean separation of concerns** — input, physics, rendering, persistence, and AI are fully independent subsystems with no cross-dependencies beyond well-defined interfaces
