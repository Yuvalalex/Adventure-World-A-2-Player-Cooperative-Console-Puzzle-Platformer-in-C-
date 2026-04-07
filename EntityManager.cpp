#include "EntityManager.h"
#include "Screen.h"
#include "Player.h"
#include "Enemy.h"
#include "BombObject.h"

void EntityManager::moveEnemies(Screen& screen, Player& p1, Player& p2) {
    auto enemies = screen.getEnemies();

    for (Enemy* enemy : enemies) {
        if (enemy && enemy->isActive()) {
            enemy->move(screen, p1, p2);
        }
    }
}

void EntityManager::updateBombs(Screen& screen, Player& p1, Player& p2, float deltaTime) {
    std::vector<BaseGameObject*> bombs = screen.getBombs();

    for (BaseGameObject* obj : bombs) {
        if (obj && obj->isActive()) {
            BombObject* bomb = static_cast<BombObject*>(obj);
            bomb->updateBomb(deltaTime, screen, p1, p2);
        }
    }
}