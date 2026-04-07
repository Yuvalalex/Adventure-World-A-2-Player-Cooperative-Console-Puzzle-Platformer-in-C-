#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"
#include "ColorManager.h"

class ExtraLifeObject : public BaseGameObject {
public:
    ExtraLifeObject(Point loc)
        : BaseGameObject(ObjectType::EXTRA_LIFE, CHAR_EXTRA_LIFE, loc,
            ColorManager::getColor(ObjectType::EXTRA_LIFE), true, true) {
    }

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {


        player.gainLife();
        player.setMessage("GAIN EXTRA LIFE!");

        this->setActive(false);
        screen.clearObjectAt(location);
        screen.updateVisuals();

        return true;
    }
};