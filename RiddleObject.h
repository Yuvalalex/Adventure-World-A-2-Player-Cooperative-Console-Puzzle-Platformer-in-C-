#pragma once
#include "BaseGameObject.h"
#include "RiddleManager.h"
#include "ColorManager.h"
#include "Game.h"

class RiddleObject : public BaseGameObject {
private:
    int riddleId;  // Memory of which riddle this is

public:
    // Constructor receives the ID and stores it
    RiddleObject(Point loc, int id)
        : BaseGameObject(ObjectType::RIDDLE, CHAR_RIDDLE, loc,
            ColorManager::getColor(ObjectType::RIDDLE), true, true), riddleId(id) {
    }

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
       
        RiddleResult result = RiddleManager::handleRiddleInteraction(player, otherPlayer, screen, this->riddleId);

        if (result.solved) {
            player.setMessage("Riddle Solved!");
            player.addScore(50);
            this->setActive(false);
            screen.clearObjectAt(location);
            return true; 
        }
        else {
            if (result.answered && !result.correct) {
                player.setMessage("Wrong Answer!");
                player.addScore(-50);
                player.stopMoving();
                return false;
            }
            else if (!result.answered) {
               
                return false;
            }
			
        }
       
        return false; 
    }
};
