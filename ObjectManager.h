#pragma once
#include "BaseGameObject.h"
#include "Constants.h"
#include <vector>
#include <memory>

// Forward declarations
class Enemy;
class RoomBoard;
class Player;

class ObjectManager {
private:
    // Fast lookup map for objects by position
    BaseGameObject* objectMap[LEVEL_HEIGHT][LEVEL_WIDTH];

    // Ownership container for all game objects
    std::vector<std::unique_ptr<BaseGameObject>> allObjects;
    Point p1Location = Point(-1, -1);
    Point p2Location = Point(-1, -1);


public:
    ObjectManager();

    void clearAll();

    // Initialize objects from board layout
    void initFromBoard(RoomBoard& board, Player* p1, Player* p2);

    // Object management
    void addObject(std::unique_ptr<BaseGameObject> obj);
    BaseGameObject* getObjectAt(Point p) const;
    void removeObjectAt(Point p);
    void moveObject(Point from, Point to);

    // Game state queries
    bool isAllSwichesOn();

	Point getPlayer1StartPos() const { return p1Location; }
	Point getPlayer2StartPos() const { return p2Location; }

    // Get specific object types
    std::vector<Enemy*> getEnemies() const;
    std::vector<BaseGameObject*> getBombs() const;

};