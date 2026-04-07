#pragma once
#include "RoomBoard.h"
#include "ObjectManager.h"
#include "BoardRenderer.h"
#include "LegendManager.h"
#include "Constants.h"
#include <string>
#include <vector>

// Forward declarations
class Player;
class Enemy;

class Screen {
private:
    RoomBoard board;
    ObjectManager objectManager;
    BoardRenderer renderer;
    LegendManager legendManager;



    Player* m_p1;
    Player* m_p2;

    bool m_isLightOn;
    int m_currentLevel;
    float m_currentTime;
    Point roomExitPos;

    Point m_p1_start_pos;
    Point m_p2_start_pos;
public:
    Screen();
    ~Screen();

    // Data accessors
    float getTime() const { return m_currentTime; }
    int getLevel() const { return m_currentLevel; }
    void setLevel(int level) { m_currentLevel = level; }

    // Level transition helpers
    void setRoomExitPos(Point p) { roomExitPos = p; }
    Point getRoomExitPos() const { return roomExitPos; }
    Point getNextRoomExitPos() const;
    Point getNextRoomEnterPos() const;
    bool legal_levState_to_go_prev() const { return m_currentLevel > 1; }

    // Lighting system
    bool isLightOn() const;
    void set_m_isLightOn(bool isOn) { m_isLightOn = isOn; }
    bool get_m_isLightOn() const { return m_isLightOn; }
    bool isLevelDark() const { return board.isLevelDark(); }
    void setBoardDarkness(bool isDark) { board.setIsDark(isDark); }

    // Board management
    bool setBoardLevel(const std::string& filename, Player* p1, Player* p2);
    void drawBoard(const Player* p1, const Player* p2);
    void updateVisuals();

    // Object management
    ObjectManager& getObjectManager() { return objectManager; }
    RoomBoard& getBoard() { return board; }
    LegendManager& getLegendManager() { return legendManager; }
    BaseGameObject* getObjectPtrAt(Point p) const;
    char getObjectAt(Point p) const;
    void removeObject(Point p);
    void dropObject(BaseGameObject* obj);
    void moveObject(Point from, Point to, BaseGameObject* obj);
    void clearObjectAt(Point p) { removeObject(p); }
    void updateObject(BaseGameObject* obj);
    bool pushObstacle(Point p, Direction dir);

    // Player movement
    void movePlayer(Point from, Point to, const Player& player);
    bool inBoard(Point p) const;

    // UI updates
    void updateStats(const Player& p1, const Player& p2, int level = 1,
        float time = 0.0f, GameStatus status = GameStatus::RUNNING);
    void reprintFullBoard(const Player& p1, const Player& p2);
    void updateAndDraw(const Player& p1, const Player& p2) { reprintFullBoard(p1, p2); }
    void updateAndDrawAfterMessage(const Player& p1, const Player& p2) { reprintFullBoard(p1, p2); }

    // Entity queries
    std::vector<Enemy*> getEnemies() const { return objectManager.getEnemies(); }
    std::vector<BaseGameObject*> getBombs() const { return objectManager.getBombs(); }
    bool getisAllSwichesOn() { return objectManager.isAllSwichesOn(); }
    void setPlayers(Player* p1, Player* p2) {
        m_p1 = p1;
        m_p2 = p2;
    }






};