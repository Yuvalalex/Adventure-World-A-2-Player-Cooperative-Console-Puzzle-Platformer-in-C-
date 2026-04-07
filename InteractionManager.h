#pragma once
#include "Player.h"
#include "BaseGameObject.h"
#include "Screen.h"

class InteractionManager {
public:
    // Handle collision between player and game object
    static bool handlePlayerVsObject(Player& player, Player& otherPlayer,
        BaseGameObject* obj, Screen& screen);

    // Handle collision between two players
    static bool handlePlayerVsPlayer(Player& movingPlayer, Player& staticPlayer);
};