#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"
#include "ColorManager.h"

class SpringObject : public BaseGameObject {
public:
    SpringObject(Point loc)
        : BaseGameObject(ObjectType::SPRING, CHAR_SPRING, loc, true, true) {}

    // Calculate launch velocity and duration based on number of compressed springs
    static std::pair<int, int> calculateLaunchParams(int springCount) {
        int velocity = springCount;
        int duration = springCount * springCount;
        return { velocity, duration };
    }

    void resetVisuals() {
        this->setIsVisible(true);
        this->setColor(ColorManager::getColor(ObjectType::SPRING));
    }

    //virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
    //    // Start compression if not already compressing
    //    if (!player.getIsCompressing()) {
    //        player.startCompressing(player.getDirection());
    //    }

    //    player.addSpring(this);
    //    player.incrementCompression();
    //    player.setMessage("Charging...");

    //    // Hide spring visually while compressed under player
    //    this->setIsVisible(false);

    //    return true;
    //}

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
        // Start compression if not already compressing
        if (!player.getIsCompressing()) {
            player.startCompressing(player.getDirection());


            player.addSpring(this);
            player.incrementCompression();
            player.setMessage("Charging...");

            // Hide spring visually while compressed under player
            this->setIsVisible(false);

            return true;
        }
        if (player.getIsCompressing() && player.getDirection() == player.getCompressionDir()) {
            // Already compressing, just add this spring
            player.addSpring(this);
            player.incrementCompression();
            player.setMessage("Charging...");
            // Hide spring visually while compressed under player
            this->setIsVisible(false);
            return true;
		}
    }
};
