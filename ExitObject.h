#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"
#include "ColorManager.h"
#include "Constants.h"

class ExitObject : public BaseGameObject {
private:
    bool isNextRoomExit; // true = next level, false = previous level

public:
    ExitObject(Point loc, bool goesToNextRoom)
        : BaseGameObject(ObjectType::EXIT, CHAR_EXIT_NEXT , loc, ColorManager::getExitColor(CHAR_EXIT_NEXT), true, true),
        isNextRoomExit(goesToNextRoom) {}

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {

        // Check if all switches are activated before allowing exit
        bool allSwitchesOn = screen.getisAllSwichesOn();
        int currLevel = screen.getLevel();
        int requiredPointsLevelForExit = currLevel * 100; // Example: level 3  require total (p1.score + p2.score) >= 300 points
        int combinedScore = player.getScore() + otherPlayer.getScore();
        bool hasRequiredPoints = (combinedScore >= requiredPointsLevelForExit);

        if (isNextRoomExit) {

            if (allSwitchesOn && !otherPlayer.getGoToPrevRoom() && hasRequiredPoints) {
                this->setSymbol(CHAR_EXIT_OPEN);
                this->setColor(Color::GREEN);
				screen.updateObject(this);

                player.setFinished(true);
                player.setMessage("EXIT ROOM!");
                player.stopMoving();
                screen.setRoomExitPos(player.getPosition());
                return true;
            }
            else {
                if (!hasRequiredPoints) {
                    player.resetMessages();
                    player.setMessage("NOT ENOUGH POINTS TO EXIT!");
                    player.stopMoving();
                    return false;
                }
                else {
                    player.resetMessages();
					player.setMessage("NOT ALL SWITCHES ON!");
                    player.stopMoving();
                    return false;
                }
               
                player.stopMoving();
                return false;
            }
        }
        else {
            // Previous room exit
            bool canGoBack = screen.legal_levState_to_go_prev();

            if (canGoBack && !otherPlayer.isFinished()) {
                this->setSymbol(CHAR_EXIT_OPEN);
                this->setColor(Color::GREEN);
                screen.updateObject(this);
              
                player.setGoToPrevRoom(true);
                player.setMessage("EXIT ROOM!");
                return true;
            }
            else {
                player.resetMessages();
                player.setMessage("CAN'T EXIT ROOM!");
                player.stopMoving();
                return false;
            }
        }
    }
};
