#pragma once
#include <memory>
#include <array>
#include "Player.h"
#include "Point.h"
#include "Constants.h"

class PlayerManager {
    // Entities
    Player p1;
    Player p2;
    Player p0_system;
    std::array<Point, 3> playerStartLocations;  // std::array of pointers

public:
    
    PlayerManager() = default;
    ~PlayerManager() = default;

    Player& getPlayer1() { return p1; }
    Player& getPlayer2() { return p2; }
    Player& getSystemPlayer() { return p0_system; }
};