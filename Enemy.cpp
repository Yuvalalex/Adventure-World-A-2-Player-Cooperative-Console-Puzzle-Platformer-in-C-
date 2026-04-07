#include "Enemy.h"
#include "Screen.h"
#include "Move.h"

#include "Player.h"
#include <cmath> 

Enemy::Enemy(Point loc)
    : BaseGameObject(ObjectType::ENEMY, CHAR_ENEMY, loc, Color::LIGHTRED, true, false) {}

void Enemy::move(Screen& screen, Player& p1, Player& p2) {
    if (!active) return;

    // 1. Select Target (Closest valid player)
    Player* target = &p1;
    int dist1 = std::abs(p1.getPosition().getX() - location.getX()) + std::abs(p1.getPosition().getY() - location.getY());
    int dist2 = std::abs(p2.getPosition().getX() - location.getX()) + std::abs(p2.getPosition().getY() - location.getY());

    if (!p2.isFinished() && p2.getLives() > 0 && (p1.isFinished() || dist2 < dist1)) {
        target = &p2;
    }

    if (target->isFinished() || target->getLives() <= 0) return;

    // Pathfinding
    Point next = location;

	// Try X Before Y
    if (target->getPosition().getX() > location.getX()) next.setX(location.getX() + 1);
    else if (target->getPosition().getX() < location.getX()) next.setX(location.getX() - 1);

    char objAtNext = screen.getObjectAt(next);
    bool isBlockedX = (objAtNext != ' ') && (next != target->getPosition());

    if (isBlockedX) {
        next = location; // Reset
		// Try Y Movement
        if (target->getPosition().getY() > location.getY()) next.setY(location.getY() + 1);
        else if (target->getPosition().getY() < location.getY()) next.setY(location.getY() - 1);
    }

    //  Execution
    if (next == target->getPosition()) {
        if (target->get_HasSword()) {
            target->setMessage("Enemy Hit By Sword!");
            this->setActive(false);
            screen.removeObject(this->getLoc());
            return;
        }
        else {
            target->setMessage("Enemy Hit!");
            target->decreaseLife();
            return;
        }
    
    }
    else if (screen.getObjectAt(next) == ' ') {
        screen.moveObject(location, next, this);
    }
}

bool Enemy::handleCollision(Player& player, Player& otherP, Screen& screen) {
    if (player.get_HasSword()) {
        player.setMessage("Enemy Hit By Sword!");
        this->setActive(false);
        screen.removeObject(this->getLoc());
        return true;
    }
    else {
        player.setMessage("Enemy Hit!");
        player.decreaseLife();
        return true;
    }
}