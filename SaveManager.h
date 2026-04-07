#pragma once
#include <string>
#include "Player.h"

// Forward declaration
class ScreenGameManager;

class SaveManager {
public:
    // Save complete game state to file
    static bool saveCompleteState(const std::string& filename,
        Player& p1, Player& p2,
        float levelTime,
        ScreenGameManager& screenManager);

    // Load complete game state from file
    static bool loadCompleteState(const std::string& filename,
        Player& p1, Player& p2,
        float& levelTime,
        ScreenGameManager& screenManager);

};