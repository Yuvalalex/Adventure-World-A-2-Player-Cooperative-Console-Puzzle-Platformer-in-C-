#pragma once
#include "Constants.h"

// Forward declarations
class Player;
class Screen;

class PhysicsManager {
public:
    // Move player with collision detection and physics simulation
    static void movePlayer(Player& player, Player& otherPlayer,
        Screen& screen, Direction dir);
};