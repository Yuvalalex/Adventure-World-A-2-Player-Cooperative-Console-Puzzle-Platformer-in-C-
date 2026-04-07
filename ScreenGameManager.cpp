#include "ScreenGameManager.h"
#include "ErrorManager.h"
#include <iostream>

// Constructor
ScreenGameManager::ScreenGameManager()
    : uiDisplayManager(std::make_shared<UIdisplayManager>()),
    menuManager(std::make_shared<MenuManager>()),
    currScreen(nullptr),
    currentLevelIdx(0),
    gameLoadSaved(false)
{
}

// Destructor
ScreenGameManager::~ScreenGameManager() {
    allScreens.clear();
}

// Load all game levels from files
void ScreenGameManager::loadAllGameLevels(Player* p1, Player* p2) {
    // Don't reload if we loaded from save file
    if (gameLoadSaved) {
        return;
    }

    try {
        // Try to load level files
        if (!levelManager.loadLevelsFromDirectory()) {
            ErrorManager::logError("ScreenGameManager: No level files found in directory.");
            std::cerr << "Error: No level files found. Please ensure level files exist." << std::endl;
            allScreens.clear();
            currScreen = nullptr;
            return;
        }

        // Get all level files
        const auto& files = levelManager.getAllFiles();
        allScreens.clear();

        // Add menu as first screen
        allScreens.push_back(menuManager);

        int levelCounter = 1;

        // Load each level file
        for (const auto& filename : files) {
            auto newScreen = std::make_shared<Screen>();
            newScreen->setLevel(levelCounter);
            levelCounter++;

            // Load the board without setting player positions yet
            if (!newScreen->setBoardLevel(filename, nullptr, nullptr)) {
                ErrorManager::logError("ScreenGameManager: Failed to parse: " + filename);
                std::cerr << "Warning: Failed to parse level file: " << filename << std::endl;
            }

            allScreens.push_back(newScreen);
        }

        // Add UI display manager at the end
        allScreens.push_back(uiDisplayManager);

        // Start at menu
        restartGame();

        // Set player positions for first level
        if (currScreen != nullptr && p1 != nullptr && p2 != nullptr) {
            bool success = currScreen->setBoardLevel(levelManager.getCurrentLevelFile(), p1, p2);
            if (!success) {
                ErrorManager::logError("ScreenGameManager: Failed to set player positions.");
                std::cerr << "Error: Failed to set player positions for first level." << std::endl;
            }
        }
        else {
            std::cerr << "Warning: currScreen or players are null during setup." << std::endl;
        }
    }
    catch (const std::exception& ex) {
        ErrorManager::logError(std::string("ScreenGameManager: Exception: ") + ex.what());
        std::cerr << "Fatal error during level loading: " << ex.what() << std::endl;
        allScreens.clear();
        currScreen = nullptr;
    }
    catch (...) {
        ErrorManager::logError("ScreenGameManager: Unknown exception during loadAllGameLevels.");
        std::cerr << "Unknown fatal error during level loading." << std::endl;
        allScreens.clear();
        currScreen = nullptr;
    }
}

// Update the current screen pointer based on currentLevelIdx
void ScreenGameManager::updateCurrentPointer() {
    if (!allScreens.empty() &&
        currentLevelIdx >= 0 &&
        currentLevelIdx < static_cast<int>(allScreens.size())) {
        currScreen = allScreens[currentLevelIdx].get();
    }
    else {
        currScreen = nullptr;
    }
}

// Restart game - go back to menu
void ScreenGameManager::restartGame() {
    levelManager.setLevelIndex(0);
    currentLevelIdx = 0;
    updateCurrentPointer();
}

// Start the first playable level
void ScreenGameManager::startFirstLevel(Player* p1, Player* p2, bool silentMode) {
    // Make sure we have levels
    if (allScreens.size() <= 1) {
        return;
    }

    // Switch from menu (index 0) to first level (index 1)
    levelManager.setLevelIndex(0);
    currentLevelIdx = 1;
    updateCurrentPointer();

    if (currScreen != nullptr && p1 != nullptr && p2 != nullptr) {
        currScreen->setBoardLevel(levelManager.getCurrentLevelFile(), p1, p2);

        if (silentMode) {
            // Show silent mode screen
            uiDisplayManager->show_UI_Screen_Msg_display(SILENT_MODE_CMP_REPO_SCREEN_FILE_NAME, Color::YELLOW);
        }
        else {
            // Normal mode - draw the board
            currScreen->reprintFullBoard(*p1, *p2);
        }
    }
}

// Move to the next level
void ScreenGameManager::moveToNextLevel() {
    if (levelManager.hasMoreLevels()) {
        levelManager.advanceLevel();
        // Add 1 to skip the menu at index 0
        currentLevelIdx = static_cast<int>(levelManager.getCurrentLevelIndex()) + 1;
        updateCurrentPointer();
    }
    else {
        // No more levels - show game over
        if (menuManager != nullptr) {
            menuManager->showGameOver(0);
        }
    }
}

// Move to the previous level
void ScreenGameManager::moveToPrevLevel() {
    if (levelManager.getCurrentLevelIndex() > 0) {
        levelManager.goBackLevel();
        // Add 1 to skip the menu at index 0
        currentLevelIdx = static_cast<int>(levelManager.getCurrentLevelIndex()) + 1;
    }
    updateCurrentPointer();
}

// Get the current screen
Screen* ScreenGameManager::getCurrentScreen() {
    return currScreen;
}

// Check if game is finished
bool ScreenGameManager::isGameFinished() const {
    return !levelManager.hasMoreLevels();
}

// Get current level number (1-based)
int ScreenGameManager::getCurrentLevelNumber() const {
    return static_cast<int>(levelManager.getCurrentLevelIndex()) + 1;
}

// Set current level by number
void ScreenGameManager::setCurrentLevelByNumber(int levelNumber) {
    // Convert from 1-based to 0-based index
    int idx = levelNumber - 1;
    if (idx < 0) {
        idx = 0;
    }

    levelManager.setLevelIndex(idx);
    // Add 1 to account for menu at slot 0
    currentLevelIdx = idx + 1;
    updateCurrentPointer();
}

// Draw the current game state
void ScreenGameManager::drawCurrentState(Player* p1, Player* p2) {
    if (currScreen == nullptr) {
        return;
    }

    if (currScreen->getTime() == 0) {
        // First draw - full repaint
        currScreen->reprintFullBoard(*p1, *p2);
    }
    else {
        // Update only changed parts
        currScreen->updateVisuals();
    }
}