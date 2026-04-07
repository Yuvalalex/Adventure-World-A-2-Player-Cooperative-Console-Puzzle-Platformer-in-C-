#include "Screen.h"
#include "Utils.h"
#include "Move.h"
#include "Player.h"
#include "FileManager.h"
#include "ErrorManager.h"
#include <iostream>

using namespace std;

Screen::Screen()
    : m_p1(nullptr), m_p2(nullptr),
    m_isLightOn(false), m_currentLevel(1), m_currentTime(0.0f),
    roomExitPos(-1, -1) {}

Screen::~Screen() {}

bool Screen::isLightOn() const {
    // Level is lit if: not dark, or either player has torch, or manual light is on
    if (!board.isLevelDark()) return true;
    if (m_p1 && m_p1->get_HasTorch()) return true;
    if (m_p2 && m_p2->get_HasTorch()) return true;
    if (m_isLightOn) return true;
    return false;
}

bool Screen::setBoardLevel(const string& filename, Player* p1, Player* p2) {
    m_p1 = p1;
    m_p2 = p2;

    objectManager.clearAll();

    if (board.loadLevelFromFile(filename)) {
        
        objectManager.initFromBoard(board, p1, p2);
        return true;
    }

    return false;
}

void Screen::updateVisuals() {}

void Screen::drawBoard(const Player* p1, const Player* p2) {
    bool isLit = isLightOn();

    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        Utils::gotoxy(0, y);

        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            Point currentPoint(x, y);

            // Draw player 1 if at this position
            if (p1 && !p1->isFinished() && p1->getPosition() == currentPoint) {
                p1->draw();
                continue;
            }

            // Draw player 2 if at this position
            if (p2 && !p2->isFinished() && p2->getPosition() == currentPoint) {
                p2->draw();
                continue;
            }

            // Draw game object at this position
            BaseGameObject* obj = objectManager.getObjectAt(currentPoint);

            if (obj && obj->getIsVisible()) {
                // Walls are always visible, even in darkness
                if (isLit  || obj->getType() == ObjectType::TORCH) {
                    Utils::setTextColor(obj->getColor());
                    cout << obj->getRepr();
                }
                else {
                    // Dark area - render as black space
                    Utils::setTextColor(Color::WHITE);
                    cout << ' ';
                }
            }
            else {
                // Empty space
                Utils::setTextColor(Color::WHITE);
                cout << ' ';
            }
        }
    }

    Utils::setTextColor(Color::WHITE);
}

void Screen::reprintFullBoard(const Player& p1, const Player& p2) {
    Utils::cls();
    legendManager.resetCache();
    drawBoard(&p1, &p2);
    legendManager.drawLegend(board);
    legendManager.updateStats(p1, p2, m_currentLevel, m_currentTime, GameStatus::RUNNING);
}

void Screen::movePlayer(Point from, Point to, const Player& player) {
    bool isLit = isLightOn();

    // Clear old position
    Utils::gotoxy(from.getX(), from.getY());
    BaseGameObject* objAtFrom = objectManager.getObjectAt(from);

    if (objAtFrom && objAtFrom->getIsVisible()) {
        if (isLit || objAtFrom->getType() == ObjectType::TORCH) {
            Utils::setTextColor(objAtFrom->getColor());
            cout << objAtFrom->getRepr();
        }
        else {
            cout << ' ';
        }
    }
    else {
        cout << ' ';
    }

    // Draw player at new position
    if (!player.isFinished() && !player.getGoToPrevRoom()) {
        Utils::gotoxy(to.getX(), to.getY());
        Utils::setTextColor(player.getColor());
        cout << player.getRepr();
        Utils::setTextColor(Color::WHITE);
    }
}

BaseGameObject* Screen::getObjectPtrAt(Point p) const {
    return objectManager.getObjectAt(p);
}

char Screen::getObjectAt(Point p) const {
    // Check for players first
    if (m_p1 != nullptr && m_p1->getPosition() == p) return CHAR_PLAYER_1;
    if (m_p2 != nullptr && m_p2->getPosition() == p) return CHAR_PLAYER_2;

    // Check for game objects
    BaseGameObject* obj = objectManager.getObjectAt(p);
    if (obj != nullptr && obj->isActive()) {
        return obj->getRepr();
    }

    // Fall back to static board character
    return board.getCharAt(p);
}

void Screen::removeObject(Point p) {
    objectManager.removeObjectAt(p);
    Utils::gotoxy(p.getX(), p.getY());
    cout << ' ';
}

void Screen::dropObject(BaseGameObject* obj) {
    if (!obj) {
        return;
    }

    objectManager.addObject(std::unique_ptr<BaseGameObject>(obj));

    Utils::gotoxy(obj->getLoc().getX(), obj->getLoc().getY());
    Utils::setTextColor(obj->getColor());
    cout << obj->getRepr();
    Utils::setTextColor(Color::WHITE);
}

void Screen::updateObject(BaseGameObject* obj) {
    if (!obj) {
        return;
    }

    bool isLit = isLightOn();
    Utils::gotoxy(obj->getLoc().getX(), obj->getLoc().getY());

    if (isLit || obj->getType() == ObjectType::TORCH ) {
        Utils::setTextColor(obj->getColor());
        cout << obj->getRepr();
    }
    else {
        cout << ' ';
    }

    Utils::setTextColor(Color::WHITE);
}

void Screen::moveObject(Point from, Point to, BaseGameObject* obj) {
    objectManager.moveObject(from, to);

    Utils::gotoxy(from.getX(), from.getY());
    cout << ' ';

    updateObject(obj);
}

bool Screen::pushObstacle(Point p, Direction dir) {
    Point nextPos = Move::calculateNextPoint(p, dir);

    if (!inBoard(nextPos)) {
        return false;
    }

    if (getObjectAt(nextPos) == ' ') {
        BaseGameObject* obj = getObjectPtrAt(p);
        moveObject(p, nextPos, obj);
        return true;
    }

    return false;
}

bool Screen::inBoard(Point p) const {
    return board.inBoard(p);
}

void Screen::updateStats(const Player& p1, const Player& p2, int level,
    float time, GameStatus status) {
    m_currentLevel = level;
    m_currentTime = time;
    legendManager.updateStats(p1, p2, level, time, status);
}

Point Screen::getNextRoomExitPos() const {
    // Find the next room exit marker on the board
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            Point p(x, y);
            if (board.getCharAt(p) == CHAR_EXIT_NEXT) {
                return p;
            }
        }
    }
    return Point(10, 10); // Default position
}

Point Screen::getNextRoomEnterPos() const {
    // Find entry point for next room (previous exit or player start)
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            Point p(x, y);
            char c = board.getCharAt(p);

            if (c == CHAR_EXIT_PREV || c == CHAR_PLAYER_1 || c == CHAR_PLAYER_2) {
                return p;
            }
        }
    }
    return Point(10, 10); // Default position
}