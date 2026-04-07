#pragma once
#include "Point.h"
#include "Constants.h"
#include <string>

class RoomBoard {
private:
    char board[LEVEL_HEIGHT][LEVEL_WIDTH];
    Point player1StartPos;
    Point player2StartPos;
    Point legendPos;
    bool isDark;

    // Internal helper to locate special markers
    void findPlayersPositions();

public:
    RoomBoard();

    // Load level data from file
    bool loadLevelFromFile(const std::string& filename);

    // Board access
    char getCharAt(Point p) const;
    void setCharAt(Point p, char c);
    bool inBoard(Point p) const;

    // Special position getters
    Point getPlayer_1_Position() const { return player1StartPos; }
    Point getPlayer_2_Position() const { return player2StartPos; }
    Point getLegendPosition() const { return legendPos; }

    // Lighting state
    bool isLevelDark() const { return isDark; }
    void setIsDark(bool d) { isDark = d; }
};