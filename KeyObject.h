#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"

class KeyObject : public BaseGameObject {
public:
    KeyObject(Point loc)
        : BaseGameObject(ObjectType::KEY, CHAR_KEY, loc, true, true) {}

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
        player.addKey();
        player.setMessage("Got Key!");

        // Remove key from game
        this->setActive(false);
        screen.clearObjectAt(location);
        screen.updateVisuals();

        return true; // Allow player to pass through
    }
};