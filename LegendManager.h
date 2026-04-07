#pragma once
#include "Player.h"
#include "RoomBoard.h"
#include "Constants.h"
#include <string>

class LegendManager {
private:
    Point legendLocation;

    // for only redraw if changed
    std::string lastLine1;
    std::string lastLine2;
    std::string lastLine3;

public:
    LegendManager() : legendLocation(-1, -1) {}

    void drawLegend(const RoomBoard& board);
    void updateStats(const Player& p1, const Player& p2, int level,
        float timeInSeconds, GameStatus status);

    void resetCache() {
        lastLine1 = "";
        lastLine2 = "";
        lastLine3 = "";
    }
};