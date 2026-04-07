#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"
#include "ColorManager.h"

class TorchObject : public BaseGameObject {
public:
    TorchObject(Point loc)
        : BaseGameObject(ObjectType::TORCH, CHAR_TORCH, loc,
            ColorManager::getColor(ObjectType::TORCH), true, true) {}

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
        // Player can only carry one torch
        if (player.get_HasTorch()) {
            return false;
        }

        player.pickTorch();
        player.setMessage("Lights ON!");

        this->setActive(false);

        // Reprint entire board to reveal previously darkened areas
        screen.reprintFullBoard(player, otherPlayer);

        return true;
    }
};