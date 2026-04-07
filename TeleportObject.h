#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"
#include <vector>
#include <cstdlib>

class TeleportObject : public BaseGameObject {
public:
    TeleportObject(Point loc)
        : BaseGameObject(ObjectType::TELEPORT, CHAR_TELEPORT, loc, true, true) {}

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
        std::vector<Point> targets;

        // Scan board to find the paired teleport destination
        for (int y = 0; y < LEVEL_HEIGHT; ++y) {
            for (int x = 0; x < LEVEL_WIDTH; ++x) {
                Point targetPoint(x, y);

                // Skip self
                if (targetPoint == location) {
                    continue;
                }
                BaseGameObject* obj = screen.getObjectPtrAt(targetPoint);
                if (obj && obj->getType() == ObjectType::TELEPORT) {
                    targets.push_back(targetPoint); 
                }
            }
        }

        if (!targets.empty()) {
            int randomIndex = rand() % targets.size();  // Found destination teleport - warp player there
            Point targetPoint = targets[randomIndex];


            screen.movePlayer(player.getPosition(), targetPoint, player);
            player.setPosition(targetPoint);
            player.stopMomentum();
            player.setMessage("Warp!");

            return false; // Stop movement after teleport
        }



        return true; // No destination found, allow normal passage
    }

    
};