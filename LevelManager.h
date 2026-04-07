#pragma once
#include <vector>
#include <string>
#include <cstddef>

class LevelManager {
private:
    std::vector<std::string> levelFiles;
    std::size_t currentLevelIndex;

public:
    LevelManager() : currentLevelIndex(0) {}

    // Load all screen files from current directory
    bool loadLevelsFromDirectory();
    bool loadLevelsFromSavedDirectory();

    // Level navigation
    std::string getCurrentLevelFile() const;
    void advanceLevel();
    bool hasMoreLevels() const;
    void reset() { currentLevelIndex = 0; }

    // Getters and setters
    std::size_t getCurrentLevelIndex() const { return currentLevelIndex; }
    void setLevelIndex(std::size_t index);
    const std::vector<std::string>& getAllFiles() const { return levelFiles; }
    void goBackLevel();
};