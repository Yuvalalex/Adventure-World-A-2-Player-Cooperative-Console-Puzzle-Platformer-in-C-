#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"
#include "ColorManager.h"

class SwordObject : public BaseGameObject {
public:
    SwordObject(Point loc)
        : BaseGameObject(ObjectType::SWORD, CHAR_SWORD, loc,
            ColorManager::getColor(ObjectType::SWORD), true, true) {}

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
        // Player can only carry one sword
        if (player.get_HasSword()) {
            return false;
        }

        player.pickSword();
        player.setMessage("Got Sword!");

        this->setActive(false);
        screen.clearObjectAt(location);
        screen.updateVisuals();

        return true;
    }
};