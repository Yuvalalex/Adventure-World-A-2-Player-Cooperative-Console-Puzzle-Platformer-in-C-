#include "PhysicsManager.h"
#include "Player.h"
#include "Screen.h"
#include "InteractionManager.h"
#include "Move.h"
#include "Constants.h"

void PhysicsManager::movePlayer(Player& player, Player& otherPlayer,
    Screen& screen, Direction dir) {
    if (player.isFinished() || player.getLives() <= 0) {
        return;
    }

    // Determine number of steps based on momentum
    int stepsToMove = player.hasMomentum() ? player.getMomentumVelocity() : 1;

    for (int step = 0; step < stepsToMove; ++step) {
        Direction moveDirection = player.hasMomentum() ? player.getMomentumDir() : dir;
        Point currentPosition = player.getPosition();
        Point nextPosition = Move::calculateNextPoint(currentPosition, moveDirection);

        // Check Board boundaries
        if (!screen.inBoard(nextPosition)) {
            if (player.getIsCompressing()) {
                player.releaseSpring(screen);
            }
            player.stopMomentum();
            return;
        }

        // Check Player vs Player collision
        if (nextPosition == otherPlayer.getPosition() && !otherPlayer.isFinished()) {
            // Handle momentum transfer
            if (player.hasMomentum()) {
                player.transferEnergyTo(otherPlayer);
                return; // Stop current player after transfer
            }

            // Normal interaction
            if (!InteractionManager::handlePlayerVsPlayer(player, otherPlayer)) {
                if (player.getIsCompressing()) {
                    player.releaseSpring(screen);
                }
                player.stopMomentum();
                return;
            }
        }

        BaseGameObject* objectAtNext = screen.getObjectPtrAt(nextPosition);

        // Check Spring compression logic
        if (player.getIsCompressing()) {
            bool isNextPositionSpring = (objectAtNext != nullptr && objectAtNext->getType() == ObjectType::SPRING);
            if (!isNextPositionSpring || player.getDirection() != player.getCompressionDir() ) {
                // Release spring when leaving spring chain
                player.releaseSpring(screen);
                return;
            }
        }

        // Check Player vs Object interaction
        bool movementAllowed = true;
        if (objectAtNext) {
            movementAllowed = InteractionManager::handlePlayerVsObject(player, otherPlayer,
                objectAtNext, screen);
        }

        // Execute move if allowed
        if (movementAllowed) {
            player.setPosition(nextPosition);

            // Remove inactive objects (like collected keys)
            if (objectAtNext && !objectAtNext->isActive()) {
                screen.removeObject(nextPosition);
            }

            if (player.hasMomentum()) {
                player.decreaseMomentumDuration();
            }
        }
        else {
            // Movement blocked
            if (player.getIsCompressing()) {
                player.releaseSpring(screen);
            }
            player.stopMomentum();
            return;
        }
    }
}