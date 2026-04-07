#include "GameSave.h"
#include "InputStrategies.h" 
#include "Utils.h"
#include <iostream>

// Constructor
GameSave::GameSave(const std::string& file) : filename(file) {
}

// Main game loop for save mode
bool GameSave::run() {
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
            }
        }
        else if (status == GameStatus::GAME_OVER) {
            handleGameOver();
        }
        else {
            status = GameStatus::MENU;
        }
    }

    // Write final events
    int totalScore = p1.getScore() + p2.getScore();
    recordManager.logEvent(gameTick, "GAME_EXIT TOTAL_SCORE: " + std::to_string(totalScore));
    recordManager.writeEndMarker(gameTick);


    onGameEnd();
    return true;
}

// Initialize save mode
void GameSave::onInit() {
    status = GameStatus::MENU;

    // Set up recording files
    recordManager.setFiles(filename);
    recordManager.startWriting();

    // Generate and save random seed
    const long seed = static_cast<long>(GetTickCount64());
    srand(static_cast<unsigned int>(seed));
    recordManager.writeSeedMarker(seed);

    // Set input strategy to record all inputs
    inputManager.setStrategy(std::make_unique<SaveStrategy>(recordManager));

    // Notify user
    std::cout << ">>> SAVE MODE ENABLED: Will record to " << filename << " <<<" << std::endl;
    Utils::sleep(500);

    status = GameStatus::MENU;
}

// Render the current screen
void GameSave::render() {
    screenGameManager.drawCurrentState(&p1, &p2);
}

// Handle frame delay
void GameSave::handleFrameDelay() {
    Utils::sleep(GAME_SPEED_MS);
}

// Called when game ends
void GameSave::onGameEnd() {
    recordManager.closeFiles();
}

// Delegate functions to base class
void GameSave::commonInit() { baseCommonInit(); }
void GameSave::handleMenu() { baseHandleMenu(); }
void GameSave::handleGameOver() { baseHandleGameOver(); }
void GameSave::handleExitGame() { baseHandleExitGame(); }
void GameSave::loadNextLevel() { baseLoadNextLevel(); }
void GameSave::loadPrevLevel() { baseLoadPrevLevel(); }
void GameSave::resetGame() { baseResetGame(); }