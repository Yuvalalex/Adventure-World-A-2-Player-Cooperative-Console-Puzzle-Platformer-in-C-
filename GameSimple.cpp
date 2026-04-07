#include "GameSimple.h"
#include "InputStrategies.h"
#include "Utils.h"
#include <iostream>

// Main game loop for simple mode
bool GameSimple::run() {
    commonInit();
    onInit();

    try {
        while (isRunning && status != GameStatus::EXIT) {
            switch (status) {
            case GameStatus::MENU:
                handleMenu();
                break;

            case GameStatus::RUNNING:
                recordManager.logEvent(gameTick, "Loop Start");

                // Inner game loop
                while (status == GameStatus::RUNNING) {
                    baseRunTick();

                    if (status != GameStatus::RUNNING) {
                        break;
                    }
                }
                break;

            case GameStatus::GAME_OVER:
                handleGameOver();
                break;

            case GameStatus::EXIT:
                // Will exit the loop
                break;

            default:
                // Unknown state - go back to menu
                std::cerr << "Warning: Unknown game status. Resetting to MENU." << std::endl;
                status = GameStatus::MENU;
                break;
            }
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        recordManager.logEvent(gameTick + 1, std::string("FATAL_ERROR: ") + ex.what());
        isRunning = false;
    }
    catch (...) {
        std::cerr << "Unknown fatal error occurred in game loop." << std::endl;
        recordManager.logEvent(gameTick + 1, "FATAL_ERROR: Unknown exception");
        isRunning = false;
    }

    int totalScore = p1.getScore() + p2.getScore();
    recordManager.logEvent(gameTick + 1, "GAME_EXIT TOTAL_SCORE: " + std::to_string(totalScore));
    onGameEnd();

    return true;
}

// Initialize simple mode
void GameSimple::onInit() {
    // Start with menu
    status = GameStatus::MENU;

    // Use keyboard input strategy
    inputManager.setStrategy(std::make_unique<KeyboardStrategy>());
}

// Render the current screen
void GameSimple::render() {
    screenGameManager.drawCurrentState(&p1, &p2);
}

// Handle frame delay
void GameSimple::handleFrameDelay() {
    Utils::sleep(GAME_SPEED_MS);
}

//  functions to base class
void GameSimple::commonInit() { baseCommonInit(); }
void GameSimple::handleMenu() { baseHandleMenu(); }
void GameSimple::handleGameOver() { baseHandleGameOver(); }
void GameSimple::handleExitGame() { baseHandleExitGame(); }
void GameSimple::loadNextLevel() { baseLoadNextLevel(); }
void GameSimple::loadPrevLevel() { baseLoadPrevLevel(); }
void GameSimple::resetGame() { baseResetGame(); }