#include "RoomBoard.h"
#include "ErrorManager.h"
#include "FileManager.h"
#include <algorithm>

using namespace std;

RoomBoard::RoomBoard()
    : player1StartPos(-1, -1),
    player2StartPos(-1, -1),
    legendPos(-1, -1),
    isDark(false) {
    // Initialize board with empty spaces
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            board[y][x] = ' ';
        }
    }
}
void RoomBoard::findPlayersPositions() {
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            char currentChar = board[y][x];

            if (currentChar == CHAR_PLAYER_1) {
                player1StartPos = Point(x, y);
                
            }
            else if (currentChar == CHAR_PLAYER_2) {
                player2StartPos = Point(x, y);
               
            }
            else if (currentChar == CHAR_LEGEND) {
                legendPos = Point(x, y);
            }
            else if (currentChar == CHAR_FOG) {
                isDark = true;
            }
        }
    }
}


bool RoomBoard::loadLevelFromFile(const string& filename) {
    try {
        vector<string> lines = FileManager::readLines(filename);
        if (lines.empty()) {
            return false;
        }

        isDark = false; // Reset darkness state for new level

        // Clear existing board
        for (int y = 0; y < LEVEL_HEIGHT; y++) {
            for (int x = 0; x < LEVEL_WIDTH; x++) {
                board[y][x] = ' ';
            }
        }

        // Parse file content into board array
        int maxLines = min(static_cast<int>(lines.size()), LEVEL_HEIGHT);
        for (int lineIndex = 0; lineIndex < maxLines; lineIndex++) {
            string currentLine = lines[lineIndex];
            int boardX = 0;

            size_t lineLength = min(currentLine.length(),
                static_cast<size_t>(LEVEL_WIDTH));
            for (size_t charIndex = 0; charIndex < lineLength; charIndex++) {
                board[lineIndex][boardX++] = currentLine[charIndex];
            }
        }

        findPlayersPositions();
        return true;
    }
    catch (...) {
        return false;
    }
}

char RoomBoard::getCharAt(Point p) const {
    if (inBoard(p)) {
        return board[p.getY()][p.getX()];
    }
    return ' ';
}

void RoomBoard::setCharAt(Point p, char c) {
    if (inBoard(p)) {
        board[p.getY()][p.getX()] = c;
    }
}

bool RoomBoard::inBoard(Point p) const {
    return p.getX() >= 0 && p.getX() < LEVEL_WIDTH &&
        p.getY() >= 0 && p.getY() < LEVEL_HEIGHT;
}