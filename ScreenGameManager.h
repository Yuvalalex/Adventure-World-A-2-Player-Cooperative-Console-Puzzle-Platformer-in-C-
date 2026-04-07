#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Screen.h"
#include "LevelManager.h"
#include "Player.h"
#include "MenuManager.h"
#include "UIdisplayManager.h"
#include "PlayerManager.h"



class ScreenGameManager {
private:
    std::shared_ptr<UIdisplayManager> uiDisplayManager;
    std::vector<std::shared_ptr<Screen>> allScreens;
    std::shared_ptr<MenuManager> menuManager;

    LevelManager levelManager;
    PlayerManager playerManager;

    Screen* currScreen;
    int currentLevelIdx;
	bool gameLoadSaved;

    void updateCurrentPointer();

public:
    ScreenGameManager();
    ~ScreenGameManager();

	void set_gameLoadSaved(bool status) { gameLoadSaved = status; }
    bool get_gameLoadSaved() const { return gameLoadSaved; }


    void loadAllGameLevels(Player* p1, Player* p2);
    void moveToNextLevel();
    void moveToPrevLevel();
    void restartGame();
    void startFirstLevel(Player* p1, Player* p2 , bool moadStatusSilent );

    Screen* getCurrentScreen();
    bool isGameFinished() const;
    int getCurrentLevelNumber() const;
    void drawCurrentState(Player* p1, Player* p2);

   
    void setCurrentLevelByNumber(int levelNumber);

    LevelManager& getLevelManager() { return levelManager; }
     std::vector<std::shared_ptr<Screen>>& getAllScreens()  { return allScreens; }
};