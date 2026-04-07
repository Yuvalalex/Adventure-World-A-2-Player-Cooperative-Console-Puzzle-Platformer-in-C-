#pragma once
#include "BaseGameObject.h"
#include "Player.h"
#include "Screen.h"
#include "Move.h"
#include <vector>

class ObstacleObject : public BaseGameObject {
private:
    int weight;

public:
    ObstacleObject(Point loc, int w = 1)
        : BaseGameObject(ObjectType::OBSTACLE, CHAR_OBSTACLE, loc,
            Color::LIGHTGREY, true, false),
        weight(w) {}

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override {
        // Determine direction of push
        Direction pushDirection = player.hasMomentum() ?
            player.getMomentumDir() : player.getDirection();

        // Calculate player's pushing force
        int playerForce = player.hasMomentum() ? player.getMomentumVelocity() : 1;

        // Build chain of contiguous obstacles
        std::vector<ObstacleObject*> obstacleChain;
        obstacleChain.push_back(this);

        int totalWeight = this->weight;
        Point nextPosition = Move::calculateNextPoint(location, pushDirection);

        // Scan for chained obstacles
        while (true) {
            if (!screen.inBoard(nextPosition)) {
                player.stopMomentum();
                return false; 
            }

            BaseGameObject* nextObj = screen.getObjectPtrAt(nextPosition);

            if (nextObj == nullptr) {
                // Check if other player blocks the empty space
                if (otherPlayer.getPosition() == nextPosition && !otherPlayer.isFinished()) {
                    player.stopMomentum();
                    return false;
                }
                // Found empty space - end of chain
                break;
            }
            else if (nextObj->getType() == ObjectType::OBSTACLE) {
                // Add next obstacle to chain
                ObstacleObject* nextObstacle = static_cast<ObstacleObject*>(nextObj);
                obstacleChain.push_back(nextObstacle);
                totalWeight += nextObstacle->weight;

                nextPosition = Move::calculateNextPoint(nextPosition, pushDirection);
            }
            else {
                // Blocked by non-pushable object
                player.stopMomentum();
                return false;
            }
        }

        // Check if player has enough force to push
        if (playerForce < totalWeight) {
            player.setMessage("Too heavy!");
            player.stopMomentum();
            return false;
        }

        // Execute push - move from last to first to avoid collisions
        for (int i = static_cast<int>(obstacleChain.size()) - 1; i >= 0; i--) {
            ObstacleObject* objToMove = obstacleChain[i];
            Point currentLoc = objToMove->getLoc();
            Point targetLoc = Move::calculateNextPoint(currentLoc, pushDirection);

            screen.moveObject(currentLoc, targetLoc, objToMove);
            objToMove->setLoc(targetLoc);
        }

        return true; // Allow player to move into first obstacle's position
    }
};