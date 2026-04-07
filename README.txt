================================================================================
                            ADVENTURE WORLD
                     C++ 2-Player Console Game
================================================================================

Student: Yuval Alexandrony
ID: 322218033
Course: C++ 

================================================================================
                              DESCRIPTION
================================================================================

Adventure World is a 2-Player Console Game - C++ programming concepts including:

- Object Oriented Design with inheritance hierarchies
- Polymorphism and virtual functions
- Smart pointers (unique_ptr, shared_ptr) for memory management
- Design patterns: Strategy, Factory, Manager patterns
- Deterministic replay system for testing

Two players must work together to navigate through challenging levels, solving
Rooms , avoiding enemies and Mines , and collecting items to reach the exit.

================================================================================
                            HOW TO BUILD
================================================================================

VISUAL STUDIO :
-------------------
1. Open MyGame.sln
2. Select Build -> Rebuild Solution 
3. Run with Debug -> Start Without Debugging


================================================================================
                            HOW TO RUN
================================================================================

BASIC MODES:
------------
game.exe                                - Normal interactive game
game.exe -save <filename>               - Record gameplay to file
game.exe -load <filename>               - Replay recorded gameplay
game.exe -load  -silent <filename>      - Fast replay for automated testing

EXAMPLE:
---------
game.exe                                - Start normal game
game.exe -save test1_filename           - Record to test1_filename.steps.txt & test1_filename.result.txt
game.exe -load test1_filename           - Replay test1_filename
game.exe -load -silent test1_filename   - Run automated test on test1_filename

================================================================================
                           FILES REQUIRED
================================================================================

LEVEL FILES (must be in executable directory):
----------------------------------------------
- adv-world_01.screen    -> Level 1 layout
- adv-world_02.screen    -> Level 2 layout
- adv-world_03.screen    -> Level 3 layout
- adv-world_04.screen    -> Level 4 layout
- adv-world_05.screen    -> Level 5 layout
- adv-world_06.screen    -> Level 6 layout

DATA FILES:
-----------
- Riddles.txt                    -> Question|Answer pairs for riddle objects 
- fileFormat.txt                 -> Save result & steps file format description

- 3 example recorded files:
  - EX_1_game.result.txt & EX_1_game.steps.txt
  - EX_2_game.result.txt & EX_2_game.steps.txt
  - EX_3_game.result.txt & EX_3_game.steps.txt

- adv-world-cmp.result.txt     -> Expected output for automated testing
- adv-world-cmp.steps.txt      -> Expected steps for automated testing
- adv-world-silent.result.txt  -> Expected silent output for automated testing
- SavedGame.txt                -> Sample txt file for save game state in simple mode

UI DISPLAY FILES:
-----------------
- theMainMenu.txt              -> Main menu screen
- gameUserInstructions.txt     -> Instructions screen
- FinishGame.txt               -> Victory screen
- GameOver.txt                 -> Game over screen
- ExitGame.txt                 -> Exit confirmation screen
- LevelUpDisplay.txt           -> Level transition (up)
- LevelDownDisplay.txt         -> Level transition (down)
- GAME_SAVED_SUCCESSFULLY.txt  -> Save confirmation screen

================================================================================
                              CONTROLS
================================================================================

PLAYER 1 ($):              PLAYER 2 (&):
-------------              -------------
W - Move Up                I - Move Up
X - Move Down              M - Move Down
A - Move Left              J - Move Left
D - Move Right             L - Move Right
S - Stop                   K - Stop
E - Drop/Use Item          O - Drop/Use Item

PLAYER 0 SYSTEM CONTROLS:
----------------
ESC - Open Pause Menu
  -> S - Save Game
  -> H - Return to Main Menu
  -> ESC - Continue Playing
REDDLE - Answer CHAR

Note: All controls input are available 

================================================================================
                            GAME OBJECTS
================================================================================

SYMBOL  NAME        DESCRIPTION
------  ----        -----------
W       Wall        Impassable obstacle
K       Key         Collect to open doors
0/D     Door        Requires number of keys to open 
@       Bomb        Explodes after 2.5 seconds, radius of 3
!       Torch       Illuminates dark rooms ( disappears when player leave the room with it )
?       Riddle      Answer correctly to pass (+50 points) / (-50 points)
#       Spring      Compress and launch (velocity = count, duration = count^2)
*       Obstacle    Pushable object
/       Switch ON   Toggle mechanism
\       Switch OFF  Toggle mechanism
X       Exit Lock   (requires: (Total Players Score >= (LevelNum*100) Score) and (all switches ON))
x       Exit Open   Return to the relevant level

---------------------------------------------------------------------------------

^       Teleport    Warps to random other teleport                   (explain in Bonus.txt)
%       Enemy       Chases nearest player, defeats with sword/bomb   (explain in Bonus.txt)
|       Sword       Defeats enemies on contact                       (explain in Bonus.txt)
.       Main        Player loses 1 life                              (explain in Bonus.txt)
+	    Extra Life  Player gains 1 life                              (explain in Bonus.txt)

SPECIAL MARKERS (in level files):
---------------------------------
L       Legend      Position for stats display
F       Fog         Indicates dark room (needs torch)
$       Player 1    Starting position
&       Player 2    Starting position

================================================================================
                           GAME MECHANICS
================================================================================

MOVEMENT:
---------
- Players move continuously in the selected direction
- Press stop key (S/K) to halt movement
- Speed: 1 cell per game cycle

SPRING PHYSICS:
---------------
- Walk into springs to compress them
- Each additional spring increases charge
- Release launches player in opposite direction
- Velocity = number of springs compressed
- Duration = springs^2

DARK ROOMS:
-----------
- Marked with 'F' in level files
- Only walls are visible without torch
- Either player holding torch illuminates the room

SCORING:
--------
- Correct riddle answer: +50 points
- Wrong riddle answer: -50 points
- Starting lives: 3 per player


================================================================================
                         DESIGN DECISIONS
================================================================================

ARCHITECTURE:
-------------
The game uses a Manager-based architecture for separation of concerns:

- Game (base class)      - Core game loop and state management
  | 
  L GameSimple           - Normal interactive gameplay
  |     L GameSave       - Adds recording functionality
  L GameLoad             - Replay recorded games
        L GameSilent     - Fast replay for testing

- ScreenGameManager      - Level transitions and screen management
- InputManager           - Input handling with Strategy pattern
- PhysicsManager         - Movement and collision detection
- EntityManager          - Enemy and bomb updates
- ObjectManager          - Game object creation and storage
- MenuManager            - UI and menu systems
- SaveManager            - Game state persistence
- RecordManager          - Gameplay recording for replay

DESIGN PATTERNS:
----------------
1. Strategy Pattern (InputManager)
   - KeyboardStrategy: Real time input
   - SaveStrategy: KeyboardStrategy + Record input to file
   - LoadStrategy: Replay from file

2. Factory Pattern (GameModeManager)
   - Creates appropriate Game subclass based on command-line args

3. Inheritance/Polymorphism (Game objects)
   - BaseGameObject -> Wall, Door, Key, Enemy, etc.
   - Virtual handleCollision() for object-specific behavior

MEMORY MANAGEMENT:
------------------
- Smart pointers (unique_ptr, shared_ptr) used throughout
- No manual new/delete in game logic

DETERMINISTIC REPLAY:
---------------------
- random seed 
- Tick-based input recording
- Position-based riddle selection (not random)

================================================================================
                         CODE ORGANIZATION
================================================================================

HEADER FILES (.h):
------------------
Contain class declarations, constants, and inline functions.
All headers use #pragma once for include guards.

SOURCE FILES (.cpp):
--------------------
Contain implementation details.

KEY FILES:
----------
Constants.h          - All enums, constants, and symbols
Game.h/cpp           - Main game loop (abstract base)
GameSimple.h/cpp     - Normal gameplay implementation
Player.h/cpp         - Player entity with inventory
Screen.h/cpp         - Level representation and rendering
BaseGameObject.h     - Base class for all game objects
ObjectManager.h/cpp  - Object creation and storage
SaveManager.h/cpp    - Save/Load functionality

================================================================================
                            TESTING
================================================================================

MANUAL TESTING CHECKLIST:
-------------------------
[ ] Menu navigation works
[ ] New game starts correctly
[ ] Both players can move independently
[ ] Key collection and door opening
[ ] Bomb placement and explosion
[ ] Spring compression and launch physics
[ ] Enemy pathfinding and combat
[ ] Riddle interaction and scoring
[ ] Switch toggling
[ ] Obstacle pushing
[ ] Teleport functionality
[ ] Dark room with torch
[ ] Save game (ESC -> S)
[ ] Load saved game (Menu -> 2)
[ ] Level progression
[ ] Game over conditions

AUTOMATED TESTING:
------------------
1. Record a playthrough:
   game.exe -save test_run

2. Create expected output:
   Copy test_run.result.txt to adv-world-cmp.result.txt

3. Run silent test:
   game.exe -load -silent test_run

4. Check output:
   "SUCCESS! Files match perfectly." = PASS

================================================================================
                          KNOWN ISSUES
================================================================================

- Windows only (uses Windows.h, conio.h)
- Console must be 80x25 characters
- Two players share one keyboard

================================================================================
                         EXTERNAL SOURCES
================================================================================

CONSOLE UTILITIES:
------------------
Functions gotoxy(), SetConsoleTextAttribute(), _kbhit(), _getch()
are based on:
- Course lab materials
- Microsoft Windows documentation
- Standard console programming techniques

AI ASSISTANCE:
--------------
- All code files have been reviewed by AI for debugging assistance and code review
- All code was verified and understood by the student
- Design decisions made independently by the student

================================================================================

Student: Yuval Alexandrony
ID: 322218033


================================================================================

