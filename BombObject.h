#pragma once
#include "BaseGameObject.h"

// Forward declarations
class Screen;
class Player;

class BombObject : public BaseGameObject {
private:
    float timer;
    bool isTicking;

    // Internal explosion logic
    void explode(Screen& screen, Player& p1, Player& p2);

public:
    BombObject(Point loc);

    void startTicking();
    void updateBomb(float deltaTime, Screen& screen, Player& p1, Player& p2);

    virtual bool handleCollision(Player& player, Player& otherPlayer, Screen& screen) override;
};

