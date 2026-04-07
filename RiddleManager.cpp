#include "RiddleManager.h"
#include "Player.h"
#include "Screen.h"
#include "Utils.h"
#include "Game.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <algorithm>
#include <cctype>

std::vector<Riddle> RiddleManager::riddles;

// Normalize strings 
static inline std::string normalizeString(const std::string& str) {
    std::string result;
    for (char ch : str) {
        if (ch == '\r' || ch == '\n') continue;
        result.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));
    }
    size_t start = 0;
    size_t end = result.size();
    while (start < end && std::isspace(static_cast<unsigned char>(result[start]))) ++start;
    while (end > start && std::isspace(static_cast<unsigned char>(result[end - 1]))) --end;
    return result.substr(start, end - start);
}

bool RiddleManager::loadRiddles(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    riddles.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        size_t delimiterPos = line.find('|');
        if (delimiterPos != std::string::npos) {
            std::string question = line.substr(0, delimiterPos);
            std::string answer = line.substr(delimiterPos + 1);

            answer = normalizeString(answer);
            while (!question.empty() && (question.back() == '\r' || question.back() == '\n')) {
                question.pop_back();
            }
            riddles.push_back({ question, answer });
        }
    }
    file.close();
    return !riddles.empty();
}

// Deterministic ID
int RiddleManager::getRiddleIdForLocation(Point p) {
    if (riddles.empty()) return 0;

   
    // To ensures that always has the same ID, both in Save and Load
    int hash = (p.getX() * 31 + p.getY() * 17);
    return abs(hash) % riddles.size();
}

RiddleResult RiddleManager::handleRiddleInteraction(Player& player, Player& otherPlayer,
    Screen& screen, int riddleId) {

    RiddleResult result;
    result.playerId = player.getId();
    result.solved = false;
    result.answered = false;

    if (riddles.empty()) {
        player.setMessage("No riddles loaded!");
        return result;
    }

    if (riddleId < 0 || riddleId >= (int)riddles.size()) riddleId = 0;
    const Riddle& currentRiddle = riddles[riddleId];
    char correctChar = currentRiddle.answer[0];

    Game* gameInstance = Game::getInstance();
    RecordManager* rm = gameInstance ? &gameInstance->getRecordManager() : nullptr;
    long currentTick = gameInstance ? gameInstance->getGameTick() : 0;

    Player& p0 = Game::getInstance()->getSystemPlayer();
    int storedInput = p0.getStoredNumber();

    result.question = currentRiddle.question;

    if (storedInput == -1) {
        player.setMessage("Q: " + currentRiddle.question );
        return result; 
    }

    char playerKeyChar = storedInput + '0';

    result.answered = true;
    result.answer = std::string(1, playerKeyChar);
    result.correct = (playerKeyChar == correctChar);

    p0.setStoredNumber(-1);

    if (result.correct) {
   
        if (rm) {
            std::string logMsg = "P" + std::to_string(result.playerId) +
                " | Q: " + currentRiddle.question +
                " | Correct: " + currentRiddle.answer +
                " | User: " + result.answer +
                (result.correct ? " [V]" : " [X]");  // Added checkmark at the end for convenience

            rm->logEvent(currentTick, logMsg);
        }
        result.solved = true;
        player.resetMessages();
        return result;
    }
    else {
        player.setMessage("WRONG!");
        if (rm) {
            std::string logMsg = "P" + std::to_string(result.playerId) +
                " | Q: " + currentRiddle.question +
                " | Correct: " + currentRiddle.answer +
                " | User: " + result.answer +
                (result.correct ? " [V]" : " [X]"); // Added checkmark at the end for convenience

            rm->logEvent(currentTick, logMsg);
        }
        player.resetMessages();
        return result;
    }
}
