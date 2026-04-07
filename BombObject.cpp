#include "BombObject.h"
#include "Screen.h"
#include "Player.h"
#include "ColorManager.h"
#include <iostream>

BombObject::BombObject(Point loc)
    : BaseGameObject(ObjectType::BOMB, CHAR_BOMB, loc, true, true),
    timer(2500.0f), isTicking(false) {}

void BombObject::startTicking() {
    isTicking = true;
    timer = 2500.0f; // 2.5 second fuse
}

void BombObject::updateBomb(float deltaTime, Screen& screen, Player& p1, Player& p2) {
    if (!active || !isTicking) {
        return;
    }

    // Visual flicker effect for ticking bomb
    static int flickerCounter = 0;
    flickerCounter++;
    this->setColor(ColorManager::getBombColor(isTicking, flickerCounter));
    screen.updateObject(this);

    // Count down timer
    timer -= deltaTime;
    if (timer <= 0) {
        explode(screen, p1, p2);
        this->setActive(false);
    }
}

void BombObject::explode(Screen& screen, Player& p1, Player& p2) {
    Point center = location;
    screen.clearObjectAt(center);

    // Explosion affects (radius of 3)
    constexpr int EXPLOSION_RADIUS = 3;

    for (int dy = -EXPLOSION_RADIUS; dy <= EXPLOSION_RADIUS; dy++) {
        for (int dx = -EXPLOSION_RADIUS; dx <= EXPLOSION_RADIUS; dx++) {
            Point explosionPoint(center.getX() + dx, center.getY() + dy);

            if (!screen.inBoard(explosionPoint)) {
                continue;
            }

            // Damage players in explosion radius
            if (p1.getPosition() == explosionPoint) {
                p1.decreaseLife();
                p1.setMessage("BOOM!");
            }
            if (p2.getPosition() == explosionPoint) {
                p2.decreaseLife();
                p2.setMessage("BOOM!");
            }

            // Destroy destroyable objects (except walls and exits)
            BaseGameObject* obj = screen.getObjectPtrAt(explosionPoint);
			if (obj && obj->getType() != ObjectType::WALL && 
                obj->getType() != ObjectType::EXIT) {
                screen.removeObject(explosionPoint);
            }
        }
    }
}

bool BombObject::handleCollision(Player& player, Player& otherPlayer, Screen& screen) {
    // If bomb is not ticking, player can pick it up
    if (!isTicking && !player.get_HasBomb()) {
        player.pickBomb();
        player.setMessage("Got Bomb!");

        this->setActive(false);
        screen.clearObjectAt(location);
        screen.updateVisuals();

        return true;
    }

    // Ticking bomb or player already has bomb - cannot pick up
    return false;
}