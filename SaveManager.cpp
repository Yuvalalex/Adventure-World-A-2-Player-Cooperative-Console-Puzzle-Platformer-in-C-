#include "SaveManager.h"
#include "ScreenGameManager.h"
#include "FileManager.h"
#include "ErrorManager.h"
#include "Screen.h"
#include "Constants.h"
#include <fstream>
#include <sstream>

using namespace std;

// Save the entire game state to a file
bool SaveManager::saveCompleteState(const string& filename, Player& p1, Player& p2,
    float levelTime, ScreenGameManager& screenManager) {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Write current level and time
    file << screenManager.getCurrentLevelNumber() << " " << levelTime << "\n";

    // Write player 1 data
    file << p1.getPosition().getX() << " " << p1.getPosition().getY() << " ";
    file << p1.getLives() << " " << p1.getScore() << " " << p1.getKeys() << " ";
    file << p1.get_HasSword() << " " << p1.get_HasTorch() << " " << p1.get_HasBomb() << "\n";

    // Write player 2 data
    file << p2.getPosition().getX() << " " << p2.getPosition().getY() << " ";
    file << p2.getLives() << " " << p2.getScore() << " " << p2.getKeys() << " ";
    file << p2.get_HasSword() << " " << p2.get_HasTorch() << " " << p2.get_HasBomb() << "\n";

    // Get only the game screens (not menu screens)
    vector<shared_ptr<Screen>> gameScreens;
    for (const auto& screen : screenManager.getAllScreens()) {
        if (screen != nullptr && screen->getLevel() > 0) {
            gameScreens.push_back(screen);
        }
    }

    file << gameScreens.size() << "\n";

    // Write each screen's board
    for (const auto& screen : gameScreens) {
        file << screen->getLevel() << "\n";

        for (int y = 0; y < LEVEL_HEIGHT; y++) {
            for (int x = 0; x < LEVEL_WIDTH; x++) {
                char c = screen->getObjectAt(Point(x, y));
                // Don't save player positions in the board
                if (c == CHAR_PLAYER_1 || c == CHAR_PLAYER_2) {
                    c = CHAR_EMPTY;
                }
                file << c;
            }
            file << "\n";
        }
    }

    file.close();
    return true;
}

// Load the entire game state from a file
bool SaveManager::loadCompleteState(const string& filename, Player& p1, Player& p2,
    float& levelTime, ScreenGameManager& screenManager) {
    // Check if file exists
    if (!FileManager::fileExists(filename)) {
        return false;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Read level and time
    int currentLevel;
    file >> currentLevel >> levelTime;

    // Reload all levels first
    screenManager.loadAllGameLevels(&p1, &p2);
    screenManager.setCurrentLevelByNumber(currentLevel);

    // Read player 1 data
    int x, y, lives, score, keys, hasSword, hasTorch, hasBomb;
    file >> x >> y >> lives >> score >> keys >> hasSword >> hasTorch >> hasBomb;

    p1.setPosition(Point(x, y));
    p1.setInitialPosition(Point(x, y));
    p1.setLives(lives);
    p1.setScore(score);
    p1.setKeys(keys);
    p1.set_HasSword(hasSword);
    p1.set_HasTorch(hasTorch);
    p1.set_HasBomb(hasBomb);

    // Save player 1 position for later
    Point savedPos1 = p1.getPosition();

    // Read player 2 data
    file >> x >> y >> lives >> score >> keys >> hasSword >> hasTorch >> hasBomb;

    p2.setPosition(Point(x, y));
    p2.setInitialPosition(Point(x, y));
    p2.setLives(lives);
    p2.setScore(score);
    p2.setKeys(keys);
    p2.set_HasSword(hasSword);
    p2.set_HasTorch(hasTorch);
    p2.set_HasBomb(hasBomb);

    // Save player 2 position for later
    Point savedPos2 = p2.getPosition();

    // Read number of screens
    int numScreens;
    file >> numScreens;
    file.ignore(); // skip newline

    // Read each screen
    const auto& allScreens = screenManager.getAllScreens();

    for (int i = 0; i < numScreens; i++) {
        int levelNum;
        file >> levelNum;
        file.ignore();

        // Find the screen with this level number
        Screen* targetScreen = nullptr;
        for (const auto& screen : allScreens) {
            if (screen != nullptr && screen->getLevel() == levelNum) {
                targetScreen = screen.get();
                break;
            }
        }

        // Read the board data
        for (int row = 0; row < LEVEL_HEIGHT; row++) {
            string line;
            getline(file, line);

            if (targetScreen != nullptr) {
                // Make sure line is long enough
                while (line.size() < LEVEL_WIDTH) {
                    line += CHAR_EMPTY;
                }

                for (int col = 0; col < LEVEL_WIDTH; col++) {
                    targetScreen->getBoard().setCharAt(Point(col, row), line[col]);
                }
            }
        }

        // Initialize objects from the loaded board
        if (targetScreen != nullptr) {
            targetScreen->setPlayers(&p1, &p2);
            targetScreen->getObjectManager().initFromBoard(targetScreen->getBoard(), &p1, &p2);

            // Restore player positions if this is the current level
            if (targetScreen->getLevel() == currentLevel) {
                p1.setPosition(savedPos1);
                p1.setInitialPosition(savedPos1);
                p2.setPosition(savedPos2);
                p2.setInitialPosition(savedPos2);
            }
        }
    }

    // Update the current screen display
    Screen* currentScreen = screenManager.getCurrentScreen();
    if (currentScreen != nullptr) {
        currentScreen->setPlayers(&p1, &p2);
        currentScreen->reprintFullBoard(p1, p2);
    }

    file.close();
    return true;
}