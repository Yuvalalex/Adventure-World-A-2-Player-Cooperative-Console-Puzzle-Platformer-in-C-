#include "LegendManager.h"
#include "Utils.h"
#include "Constants.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

void LegendManager::drawLegend(const RoomBoard& board) {
    legendLocation = board.getLegendPosition();
}

void LegendManager::updateStats(const Player& p1, const Player& p2, int level,
    float timeInSeconds, GameStatus status) {

    // Default position: Top-Left
    int x = 0;
    int y = 0;

    // Use legend position from board if available
    if (legendLocation.getX() != -1) {
        x = legendLocation.getX();
        y = legendLocation.getY();
    }

    // Format game status
    string statusStr = "UNKNOWN";
    switch (status) {
    case GameStatus::RUNNING:   statusStr = "RUNNING"; break;
    case GameStatus::MENU:      statusStr = "MENU"; break;
    case GameStatus::GAME_OVER: statusStr = "GAME OVER"; break;
    case GameStatus::EXIT:      statusStr = "EXITING"; break;
    }

    // Format time as MM:SS
    int minutes = static_cast<int>(timeInSeconds) / 60;
    int seconds = static_cast<int>(timeInSeconds) % 60;

    stringstream timeStream;
    timeStream << setfill('0') << setw(2) << minutes << ":" << setw(2) << seconds;


    // Build status line
    string line1 = "LEVEL: " + to_string(level) +
        " | TIME: " + timeStream.str() +
        " | STATUS: " + statusStr;
        
    string padding(LEVEL_WIDTH, ' ');

    // Build Player 1 stats
    string line2 = "P1: L=" + to_string(p1.getLives()) +
        " S=" + to_string(p1.getScore()) +
        " K=" + to_string(p1.getKeys());
    if (p1.get_HasTorch()) line2 += " [T]";
    if (p1.get_HasBomb())  line2 += " [B]";
    if (p1.get_HasSword()) line2 += " [S]";
    if (!p1.getMessage().empty()) line2 += " | " + p1.getMessage();
    if (!p1.getAnswer().empty()) line2 += " " + p1.getAnswer();

    // Build Player 2 stats
    string line3 = "P2: L=" + to_string(p2.getLives()) +
        " S=" + to_string(p2.getScore()) +
        " K=" + to_string(p2.getKeys());
    if (p2.get_HasTorch()) line3 += " [T]";
    if (p2.get_HasBomb())  line3 += " [B]";
    if (p2.get_HasSword()) line3 += " [S]";
    if (!p2.getMessage().empty()) line3 += " | " + p2.getMessage();
    if (!p2.getAnswer().empty()) line3 += " " + p2.getAnswer();

    Utils::setTextColor(Color::WHITE);

    // only print lines if changed
    if (line1 != lastLine1) {
        Utils::gotoxy(x, y);
        cout << (line1 + padding).substr(0, LEVEL_WIDTH);
        lastLine1 = line1;
    }
    if (line2 != lastLine2) {
        Utils::gotoxy(x, y + 1);
        cout << (line2 + padding).substr(0, LEVEL_WIDTH);
        lastLine2 = line2;
    }
    if (line3 != lastLine3) {
        Utils::gotoxy(x, y + 2);
        cout << (line3 + padding).substr(0, LEVEL_WIDTH);
        lastLine3 = line3;
    }
}