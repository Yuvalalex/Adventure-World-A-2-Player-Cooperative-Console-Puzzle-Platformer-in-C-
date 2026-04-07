#include "LevelManager.h"
#include "FileManager.h"
#include "ErrorManager.h"
#include <algorithm>

bool LevelManager::loadLevelsFromDirectory( ) {
    try {
		
        // Find all files with .screen 
        levelFiles = FileManager::getFileNames(".screen");

        // Sort files alphabetically to ensure correct order
        std::sort(levelFiles.begin(), levelFiles.end());

        return !levelFiles.empty();
    }
    catch (const std::exception& e) {
        ErrorManager::logError(e.what());
        return false;
    }
}

bool LevelManager::loadLevelsFromSavedDirectory() {
    try {

        // Find all files with .screenSave 
        levelFiles = FileManager::getFileNames(".screenSave");

        // Sort files alphabetically to ensure correct order
        std::sort(levelFiles.begin(), levelFiles.end());

        return !levelFiles.empty();
    }
    catch (const std::exception& e) {
        ErrorManager::logError(e.what());
        return false;
    }
}

std::string LevelManager::getCurrentLevelFile() const {
    if (currentLevelIndex < levelFiles.size()) {
        return levelFiles[currentLevelIndex];
    }
    return "";
}

void LevelManager::advanceLevel() {
    // Clamp to [0, levelFiles.size()] so callers can safely use hasMoreLevels()
    if (currentLevelIndex < levelFiles.size()) {
        ++currentLevelIndex;
    }
}

bool LevelManager::hasMoreLevels() const {
    // True if the current index points to a valid level.
    return currentLevelIndex < levelFiles.size();
}

void LevelManager::goBackLevel() {
    if (currentLevelIndex > 0) {
        currentLevelIndex--;
    }
}

void LevelManager::setLevelIndex(size_t index) {
    if (index < levelFiles.size()) {
        currentLevelIndex = index;
    }
}