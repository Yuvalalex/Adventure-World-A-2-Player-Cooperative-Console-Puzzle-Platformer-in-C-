#pragma once
#include "Point.h"
#include "Constants.h"

class Move {
public:
    // Determine which player (P1 or P2) pressed the key, or P0 for system keys
    static int getPlayerIdFromKey(char key);

    // Convert key press to direction
    static Direction getDirectionFromKey(char key);

    // Calculate next position given current point and direction
    static Point calculateNextPoint(Point current, Direction dir);

    // Get opposite direction (useful for bouncing, springs)
    static Direction getOppositeDirection(Direction dir);
};