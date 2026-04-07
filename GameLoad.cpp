#include "GameLoad.h"
#include "InputStrategies.h"
#include "ErrorManager.h"
#include "FileManager.h"
#include "Utils.h"
#include <iostream>
#include <conio.h>

// Constructor
GameLoad::GameLoad(const std::string& file, bool silentMode)
    : filename(file),
    isSilentMode(silentMode),
    sleepSpeed(silentMode ? GAME_SPEED_MS_LOAD_SILENT_DEFAULT : GAME_SPEED_MS_LOAD_DEFAULT)
{
    if (isSilentMode) {
        Utils::setSilentMode(true);
    }
}

// Initialize load mode
void GameLoad::onInit() {
    // Copy expected results file
    FileManager::copyFile(filename + ".result.txt", RESULT_SAVE_FILE_NAME);

    // Set up recording files for reading
    recordManager.setFiles(filename);
    recordManager.startReading();

    // Use the recorded seed
    if (recordManager.getRngSeed() >= 0) {
        srand(static_cast<unsigned int>(recordManager.getRngSeed()));
    }

    // Set input strategy to load from recorded steps
    inputManager.setStrategy(std::make_unique<LoadStrategy>(recordManager));

    // Skip menu and start running
    status = GameStatus::RUNNING;
    resetGame();

    // Normal load mode - show the game
    screenGameManager.startFirstLevel(&p1, &p2, false);

    Screen* currentScreen = screenGameManager.getCurrentScreen();
    if (currentScreen != nullptr) {
        currentScreen->reprintFullBoard(p1, p2);
    }

    std::cout << ">>> LOAD MODE: Replaying " << filename << " <<<" << std::endl;
    Utils::sleep(1000);
    
}

// Render the current screen
void GameLoad::render() {
   

    screenGameManager.drawCurrentState(&p1, &p2);
}

// Handle frame delay
void GameLoad::handleFrameDelay() {
    Utils::sleep(sleepSpeed);
}

// Handle menu - just exit in load mode
void GameLoad::handleMenu() {
    status = GameStatus::EXIT;
}

// Check if replay is finished
bool GameLoad::shouldExitRunLoop() const {
    return recordManager.isReplayFullyConsumed(gameTick);
}

// Initialize common settings
void GameLoad::commonInit() {
    try {
        system("mode con: cols=80 lines=25");
        Utils::hideCursor();

        // Use the recorded seed if available
        int seed = recordManager.getRngSeed();
        if (seed <  0) {
            srand(static_cast<unsigned int>(seed));
            recordManager.logEvent(0, "Random Seed: " + std::to_string(seed));
        }
        else {
            srand(12345);
            recordManager.logEvent(0, "Random Seed: 12345");
        }

        Utils::setUseColor(true);
        screenGameManager.loadAllGameLevels(&p1, &p2);
        RiddleManager::loadRiddles(RIDDLE_FILE_NAME);
    }
    catch (const std::exception& e) {
        ErrorManager::logError(e.what());
    }
}

// Handle game over - just exit
void GameLoad::handleGameOver() {
    status = GameStatus::EXIT;
}

// Handle exit game
void GameLoad::handleExitGame() {
    recordManager.logEvent(gameTick, "GAME_EXIT");
    Utils::setSilentMode(false);

    // Compare results in silent mode
    if (isSilentMode) {
        recordManager.compareResults(RESULT_SAVE_FILE_NAME);
    }

    baseHandleExitGame();

    // Show comparison result in silent mode
    if (isSilentMode) {
        Utils::cls();
        Utils::gotoxy(0, 0);
        Utils::printTextFile(SILENT_MODE_CMP_REPO_SCREEN_FILE_NAME);
        _getch();
    }
}

// Delegate functions to base class
void GameLoad::loadNextLevel() { baseLoadNextLevel(); }
void GameLoad::loadPrevLevel() { baseLoadPrevLevel(); }
void GameLoad::resetGame() { baseResetGame(); }

// Main game loop for load mode
bool GameLoad::run() {
    commonInit();
    onInit();
   
    while (isRunning && status != GameStatus::EXIT) {
        if (status == GameStatus::MENU) {
            handleMenu();
        }
        else if (status == GameStatus::RUNNING) {
            recordManager.logEvent(gameTick, "Loop Start");

            // Inner game loop
            while (status == GameStatus::RUNNING) {
                baseRunTick();

                if (status != GameStatus::RUNNING) {
                    break;
                }

                // Check if replay is done
                if (shouldExitRunLoop()) {
                    status = GameStatus::EXIT;
                    break;
                }
            }
        }
        else if (status == GameStatus::GAME_OVER) {
            handleGameOver();
        }
        else {
            status = GameStatus::MENU;
        }
    }

    onGameEnd();
    handleExitGame();

    return true;
}