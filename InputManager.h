#pragma once
#include <memory>
#include "Constants.h"

// Forward declarations instead of heavy includes
class InputStrategy;
class MenuManager;
class Player;
class EntityManager;
class Screen;
class ScreenGameManager;

class InputManager {
private:
    MenuManager& menuManager;
    std::unique_ptr<InputStrategy> strategy;

    void processKey(char key, Player& p1, Player& p2, EntityManager& entityManager,
        Screen& screen, GameStatus& status, ScreenGameManager& screenManager,
        float levelTime, long gameTick);

public:
    InputManager(MenuManager& mm);
    ~InputManager();
    void setStrategy(std::unique_ptr<InputStrategy> newStrategy);
    bool shouldExit() const;
    void handleInput(long gameTick, Player& p1, Player& p2,
        EntityManager& entityManager, Screen& screen,
        GameStatus& status, ScreenGameManager& screenManager, float levelTime);
};
