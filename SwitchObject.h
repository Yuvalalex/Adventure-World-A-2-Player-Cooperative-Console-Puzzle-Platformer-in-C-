#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"
#include "ColorManager.h"

class SwitchObject : public BaseGameObject {
private:
    bool isOn;

public:
    SwitchObject(Point loc, bool startOn)
        : BaseGameObject(ObjectType::SWITCH,
            startOn ? CHAR_SWITCH_ON : CHAR_SWITCH_OFF,
            loc, ColorManager::getSwitchColor(startOn), true, true),
        isOn(startOn) {}

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
        // Toggle switch state
        isOn = !isOn;
        setSymbol(isOn ? CHAR_SWITCH_ON : CHAR_SWITCH_OFF);
        setColor(ColorManager::getSwitchColor(isOn));

        player.setMessage(isOn ? "Switch ON" : "Switch OFF");
        screen.updateObject(this);

        return true; // Allow passage
    }
};