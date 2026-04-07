#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"
#include "ColorManager.h"

class MineObject : public BaseGameObject {
public:
    MineObject(Point loc)
        : BaseGameObject(ObjectType::MINE, CHAR_MINE, loc,
            ColorManager::getColor(ObjectType::MINE), true, true) {
    }

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
        

        player.decreaseLife();
        player.setMessage("Hit MINE!");

        this->setActive(false);
        screen.clearObjectAt(location);
        screen.updateVisuals();

        return true;
    }
};