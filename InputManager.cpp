#include "InputManager.h"
#include "Move.h"
#include "SaveManager.h"
#include "Utils.h"
#include "InputStrategies.h"
#include "Game.h" 

InputManager::InputManager(MenuManager& mm) : menuManager(mm) {
    strategy = std::make_unique<KeyboardStrategy>();
}

InputManager::~InputManager() = default;

void InputManager::setStrategy(std::unique_ptr<InputStrategy> newStrategy) {
    strategy = std::move(newStrategy);
}

bool InputManager::shouldExit() const {
    return strategy ? strategy->shouldExit() : false;
}

void InputManager::handleInput(long gameTick, Player& p1, Player& p2,
    EntityManager& entityManager, Screen& screen,
    GameStatus& status, ScreenGameManager& screenManager, float levelTime) {

    char key = 0;
    while (strategy->fetchKey(key, gameTick)) {
        processKey(key, p1, p2, entityManager, screen, status,
            screenManager, levelTime, gameTick);
    }
}

// Process individual key input
void InputManager::processKey(char key, Player& p1, Player& p2, EntityManager& entityManager,
    Screen& screen, GameStatus& status, ScreenGameManager& screenManager,
    float levelTime, long gameTick) {

    int playerId = Move::getPlayerIdFromKey(key);
    Direction dir = Move::getDirectionFromKey(key);

    if (playerId == 0) {

        if (isdigit(key)) {
            int numVal = key - '0';

            // Store in system player
            Game::getInstance()->getSystemPlayer().setStoredNumber(numVal);
        }

        if (dir == Direction::ESCAPE) {
            PauseAction action = menuManager.showPauseMenu();
            if (action == PauseAction::SAVE) {
                if (SaveManager::saveCompleteState(STATE_SAVE_FILE, p1, p2,
                    levelTime, screenManager)) {
                    menuManager.showGameSavedMasg();
                    Utils::sleep(500);
                    screen.reprintFullBoard(p1, p2);
                }
            }
            else if (action == PauseAction::EXIT_TO_MENU) {
                status = GameStatus::MENU;
            }
            else if (action == PauseAction::CONTINUE) {
                screen.reprintFullBoard(p1, p2);
            }
        }
    }
    else if (playerId == 1 && !p1.isFinished()) {
        p1.resetMessages();
        if (dir != Direction::DISPOSE && dir != Direction::NONE) p1.setDirection(dir);
    }
    else if (playerId == 2 && !p2.isFinished()) {
        p2.resetMessages();
        if (dir != Direction::DISPOSE && dir != Direction::NONE) p2.setDirection(dir);
    }

    if (dir == Direction::DISPOSE) {
        if (playerId == 1 && !p1.isFinished()) p1.handleDispose(screen, p2);
        if (playerId == 2 && !p2.isFinished()) p2.handleDispose(screen, p1);
    }
}
