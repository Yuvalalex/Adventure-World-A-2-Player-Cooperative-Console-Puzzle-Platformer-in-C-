#include "ObjectManager.h"
#include "RoomBoard.h"
#include "Player.h"
#include "Point.h"

// Object includes
#include "Wall.h"
#include "Enemy.h"
#include "ExtraLifeObject.h"
#include "KeyObject.h"
#include "DoorObject.h"
#include "BombObject.h"
#include "TorchObject.h"
#include "TeleportObject.h"
#include "SpringObject.h"
#include "SwitchObject.h"
#include "ExitObject.h"
#include "SwordObject.h"
#include "ObstacleObject.h"
#include "RiddleObject.h"  
#include "SwordObject.h"
#include "MineObject.h"

ObjectManager::ObjectManager() {
    clearAll();
}

void ObjectManager::clearAll() {
    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            objectMap[y][x] = nullptr;
        }
    }
    allObjects.clear();
    
    p1Location = Point(-1, -1);
    p2Location = Point(-1, -1);
}

void ObjectManager::initFromBoard(RoomBoard& board, Player* p1, Player* p2) {
    clearAll();

    

    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            Point currentPoint(x, y);
            char currentChar = board.getCharAt(currentPoint);

            if (currentChar == CHAR_PLAYER_1) {
                p1Location = currentPoint;
                board.setCharAt(currentPoint, ' ');
                continue;
            }

            if (currentChar == CHAR_PLAYER_2) {
                p2Location = currentPoint;
                board.setCharAt(currentPoint, ' ');
                continue;
            }

            std::unique_ptr<BaseGameObject> newObject = nullptr;

            if (isdigit(currentChar)) {
                newObject = std::make_unique<DoorObject>(currentPoint, currentChar - '0');
            }
            else {
                switch (currentChar) {
               
                case CHAR_ENEMY:      newObject = std::make_unique<Enemy>(currentPoint); break;
                case CHAR_WALL:       newObject = std::make_unique<Wall>(currentPoint); break;
                case CHAR_KEY:        newObject = std::make_unique<KeyObject>(currentPoint); break;
                case CHAR_BOMB:       newObject = std::make_unique<BombObject>(currentPoint); break;
                case CHAR_TORCH:      newObject = std::make_unique<TorchObject>(currentPoint); break;
                case CHAR_TELEPORT:   newObject = std::make_unique<TeleportObject>(currentPoint); break;
                case CHAR_SPRING:     newObject = std::make_unique<SpringObject>(currentPoint); break;
                case CHAR_DOOR:       newObject = std::make_unique<DoorObject>(currentPoint); break;
                case CHAR_OBSTACLE:   newObject = std::make_unique<ObstacleObject>(currentPoint); break;
                case CHAR_SWORD:      newObject = std::make_unique<SwordObject>(currentPoint); break;
                case CHAR_SWITCH_ON:  newObject = std::make_unique<SwitchObject>(currentPoint, true); break;
                case CHAR_SWITCH_OFF: newObject = std::make_unique<SwitchObject>(currentPoint, false); break;
                case CHAR_EXIT_NEXT:  newObject = std::make_unique<ExitObject>(currentPoint, true); break;
                case CHAR_EXIT_PREV:  newObject = std::make_unique<ExitObject>(currentPoint, false); break;

                
                case CHAR_MINE:       newObject = std::make_unique<MineObject>(currentPoint); break;
                case CHAR_EXTRA_LIFE:       newObject = std::make_unique<ExtraLifeObject>(currentPoint); break;



                    // creating riddle with specific ID
                case CHAR_RIDDLE:
                {
                    // 1. Get RIDDLE ID based on location
                    int specificId = RiddleManager::getRiddleIdForLocation(currentPoint);

                    // 2. Create object with the ID
                    newObject = std::make_unique<RiddleObject>(currentPoint, specificId);
                    break;
                }
                }
            }

            if (newObject != nullptr) {
                addObject(std::move(newObject));
                board.setCharAt(currentPoint, ' ');
            }
        }
    }

    if (p1 && p1Location.getX() != -1 && p1Location.getY() != -1) {
        p1->setInitialPosition(p1Location);
        p1->setPosition(p1Location);
    }

    if (p2 && p2Location.getX() != -1 && p2Location.getY() != -1) {
        p2->setInitialPosition(p2Location);
        p2->setPosition(p2Location);
    }
}


void ObjectManager::addObject(std::unique_ptr<BaseGameObject> obj) {
    if (!obj) return;
    Point position = obj->getLoc();
    if (position.getX() >= 0 && position.getX() < LEVEL_WIDTH &&
        position.getY() >= 0 && position.getY() < LEVEL_HEIGHT) {
        objectMap[position.getY()][position.getX()] = obj.get();
        allObjects.push_back(std::move(obj));
    }
}

BaseGameObject* ObjectManager::getObjectAt(Point p) const {
    if (p.getX() < 0 || p.getX() >= LEVEL_WIDTH ||
        p.getY() < 0 || p.getY() >= LEVEL_HEIGHT) return nullptr;
    BaseGameObject* obj = objectMap[p.getY()][p.getX()];
    if (obj && !obj->isActive()) return nullptr;
    return obj;
}

void ObjectManager::removeObjectAt(Point p) {
    BaseGameObject* obj = getObjectAt(p);
    if (obj) {
        obj->setActive(false);
        objectMap[p.getY()][p.getX()] = nullptr;
    }
}

void ObjectManager::moveObject(Point from, Point to) {
    BaseGameObject* obj = getObjectAt(from);
    if (!obj) return;
    objectMap[from.getY()][from.getX()] = nullptr;
    objectMap[to.getY()][to.getX()] = obj;
    obj->setLoc(to);
}

std::vector<Enemy*> ObjectManager::getEnemies() const {
    std::vector<Enemy*> enemies;
    for (const auto& obj : allObjects) {
        if (obj->isActive() && obj->getType() == ObjectType::ENEMY) {
            enemies.push_back(static_cast<Enemy*>(obj.get()));
        }
    }
    return enemies;
}

std::vector<BaseGameObject*> ObjectManager::getBombs() const {
    std::vector<BaseGameObject*> bombs;
    for (const auto& obj : allObjects) {
        if (obj->isActive() && obj->getType() == ObjectType::BOMB) {
            bombs.push_back(obj.get());
        }
    }
    return bombs;
}

bool ObjectManager::isAllSwichesOn() {
    int switchCount = 0;
    int switchOnCount = 0;
    for (const auto& obj : allObjects) {
        if (obj->isActive() && obj->getType() == ObjectType::SWITCH) {
            switchCount++;
            if (obj->getRepr() == CHAR_SWITCH_ON) {
                switchOnCount++;
            }
        }
    }
    return (switchCount == 0) || (switchCount == switchOnCount);
}