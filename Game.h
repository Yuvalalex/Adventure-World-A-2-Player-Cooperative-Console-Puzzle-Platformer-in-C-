#pragma once
#include <memory>
#include <string>
#include "ScreenGameManager.h"
#include "Player.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "EntityManager.h"
#include "RiddleManager.h"
#include "RecordManager.h" 
#include "Constants.h"

class Game {
protected:
    static Game* instance;
    
    // Managers
    ScreenGameManager screenGameManager;
    MenuManager menuManager;
    InputManager inputManager;
    PhysicsManager physicsManager;
    EntityManager entityManager;
    RecordManager recordManager;

    // Entities
    Player p1;
    Player p2;
    Player p0_system;

    // State
    GameStatus status;
    GameMode mode;  
    long gameTick;
    float levelTime;
    long gameSleep;
    bool isRunning;
   
	int enemyMoveCounter;

public:
    Game();
    virtual ~Game(); 

    static Game* getInstance() { return instance; }
    RecordManager& getRecordManager() { return recordManager; }
    Player& getSystemPlayer() { return p0_system; }
    long getGameTick() const { return gameTick; }
    static float getCurrentTime();

    void setGameMode(GameMode gm) { mode = gm; }
    GameMode getMode() const { return mode; }
    virtual bool run() = 0;

protected:
    // virtual for derived classes to implement
    virtual void onInit() = 0;
    virtual void render() = 0;
    virtual void handleFrameDelay() = 0;
    virtual void onGameEnd() {}

	// Run virtua policy 
	virtual void onTickBegin() {}
	virtual void onTickEnd() {}
	virtual bool shouldExitRunLoop() const { return false; }
	virtual bool shouldRenderUI() const { return true; }

    // Internal functions
    virtual void commonInit() = 0;
    virtual void handleMenu() = 0;
    virtual void handleGameOver() = 0;
    virtual void handleExitGame() = 0;
    virtual void loadNextLevel() = 0;
    virtual void loadPrevLevel() = 0;
    virtual void resetGame() = 0;

	// Base implementations reusable by modes
	void baseCommonInit();
	void baseHandleMenu();
	void baseHandleGameOver();
	void baseHandleExitGame();
	void baseLoadNextLevel();
	void baseLoadPrevLevel();
	void baseResetGame();
	bool baseRunTick();
};


