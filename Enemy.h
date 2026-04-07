#pragma once
#include "BaseGameObject.h"

// Forward declarations
class Screen;
class Player;

class Enemy : public BaseGameObject {
public:
    Enemy(Point loc);

    // movement logic
    void move(Screen& screen, Player& p1, Player& p2);

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override;
};