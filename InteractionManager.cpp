#include "InteractionManager.h"

bool InteractionManager::handlePlayerVsObject(Player& player, Player& otherPlayer,
    BaseGameObject* obj, Screen& screen) {
    if (!obj || !obj->isActive()) {
        return true;
    }

    // Delegate collision logic to the specific object
    return obj->handleCollision(player, otherPlayer, screen);
}

bool InteractionManager::handlePlayerVsPlayer(Player& movingPlayer, Player& standingPlayer) {
    // Momentum transfer logic: if moving player has Momentum, push the Momentum to other one
    if (movingPlayer.hasMomentum()) {
        standingPlayer.setMomentum(movingPlayer.getMomentumVelocity(),
            movingPlayer.getVelocityDuration(),
            movingPlayer.getMomentumDir());
        movingPlayer.stopMomentum(); // Impact stops the moving player
        return false; // Collision occurred
    }
    else {
        // Momentum transfer logic: if moving player has no Momentum, push the Momentum by 1 to other one
        standingPlayer.setMomentum(2, 1,movingPlayer.getDirection());
        movingPlayer.stopMomentum(); // Impact stops the moving player
        return false; // Collision occurred
        
    }
    return false; // Standard blocking
}