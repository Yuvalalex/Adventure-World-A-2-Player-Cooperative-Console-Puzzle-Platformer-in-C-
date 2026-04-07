#include "Move.h"
#include <string>
#include <cctype>

int Move::getPlayerIdFromKey(char key) {
    key = std::toupper(static_cast<unsigned char>(key));

    // Player 1 controls: W, A, X, D, S, E
    if (std::string("WAXDSE").find(key) != std::string::npos) {
        return 1;
    }

    // Player 2 controls: I, J, M, L, K, O
    if (std::string("IJMLKO").find(key) != std::string::npos) {
        return 2;
    }

    // System key (like ESC)
    return 0;
}

Direction Move::getDirectionFromKey(char key) {
    key = std::toupper(static_cast<unsigned char>(key));

    // Player 1 Controls
    if (key == 'W') return Direction::UP;
    if (key == 'X') return Direction::DOWN;
    if (key == 'A') return Direction::LEFT;
    if (key == 'D') return Direction::RIGHT;
    if (key == 'S') return Direction::STAY;
    if (key == 'E') return Direction::DISPOSE;

    // Player 2 Controls
    if (key == 'I') return Direction::UP;
    if (key == 'M') return Direction::DOWN;
    if (key == 'J') return Direction::LEFT;
    if (key == 'L') return Direction::RIGHT;
    if (key == 'K') return Direction::STAY;
    if (key == 'O') return Direction::DISPOSE;

    // System Controls
    if (key == 27) return Direction::ESCAPE; // ESC key

    return Direction::INVALID;
}

Point Move::calculateNextPoint(Point current, Direction dir) {
    int x = current.getX();
    int y = current.getY();

    switch (dir) {
    case Direction::UP:    y--; break;
    case Direction::DOWN:  y++; break;
    case Direction::LEFT:  x--; break;
    case Direction::RIGHT: x++; break;
    default: break;
    }

    return Point(x, y);
}

Direction Move::getOppositeDirection(Direction dir) {
    switch (dir) {
    case Direction::UP:    return Direction::DOWN;
    case Direction::DOWN:  return Direction::UP;
    case Direction::LEFT:  return Direction::RIGHT;
    case Direction::RIGHT: return Direction::LEFT;
    default: return Direction::NONE;
    }
}