#pragma once
#include "Point.h"
#include "Constants.h"
#include "ColorManager.h"

// Forward declarations to avoid circular dependencies
class Player;
class Screen;

class BaseGameObject {
protected:
    Point location;
    char symbol;
    char screenRepresentation;
    Color color;
    ObjectType type;
    bool isVisible;
    bool isPassable;
    bool active;

public:
    // Constructor with automatic color assignment
    BaseGameObject(ObjectType t, char c, Point loc, bool vis = true, bool pass = false)
        : type(t), symbol(c), screenRepresentation(c), location(loc),
        isVisible(vis), isPassable(pass), active(true)
    {
        this->color = ColorManager::getColor(t);
    }

    // Constructor with manual color specification
    BaseGameObject(ObjectType t, char c, Point loc, Color col, bool vis = true, bool pass = false)
        : type(t), symbol(c), screenRepresentation(c), location(loc), color(col),
        isVisible(vis), isPassable(pass), active(true) {}

    virtual ~BaseGameObject() {}

    // Position accessors
    Point getLoc() const { return location; }
    void setLoc(Point p) { location = p; }

    // Visual properties
    Color getColor() const { return color; }
    void setColor(Color c) { color = c; }
    char getRepr() const { return screenRepresentation; }
    void setSymbol(char c) { symbol = c; screenRepresentation = c; }

    // Object properties
    ObjectType getType() const { return type; }
    bool isActive() const { return active; }
    void setActive(bool a) { active = a; }
    bool getIsVisible() const { return isVisible; }
    void setIsVisible(bool v) { isVisible = v; }
    bool getIsPassable() const { return isPassable; }

    // Collision interface - override in derived classes for custom behavior
    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) {
        return isPassable;
    }

    // Update interface - for objects that need per-frame updates
    virtual void update(float deltaTime) {}
};