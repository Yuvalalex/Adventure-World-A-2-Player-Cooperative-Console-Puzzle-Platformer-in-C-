#include "Game.h"
#include "Utils.h"
#include "ErrorManager.h"
#include "SaveManager.h"

// Singleton instance pointer
Game* Game::instance = nullptr;

// Constructor - initialize all members
Game::Game()
    : p1(0, 0, CHAR_PLAYER_1, 1, Color::LIGHTCYAN),
    p2(0, 0, CHAR_PLAYER_2, 2, Color::LIGHTMAGENTA),
    p0_system(0, 0, ' ', 0, Color::WHITE),
    inputManager(menuManager),
    status(GameStatus::MENU),
    gameTick(0),
    levelTime(0.0f),
    gameSleep(0),
    isRunning(true),
    mode(GameMode::SIMPEL),
    enemyMoveCounter(0)
{
    instance = this;
}

// Destructor
Game::~Game() {
    instance = nullptr;
}

// Get current game time
float Game::getCurrentTime() {
    return 0.0f;
}

// Initialize common game settings
void Game::baseCommonInit() {
    try {
        // Set console size
        system("mode con: cols=80 lines=25");
        Utils::hideCursor();

        // Set random seed
        srand(GAME_SEED_DEFAULT);
        recordManager.logEvent(0, "Random Seed: 12345");

        // Enable colors
        Utils::setUseColor(true);

        // Load all game levels
        screenGameManager.loadAllGameLevels(&p1, &p2);

        // Load riddles
        RiddleManager::loadRiddles(RIDDLE_FILE_NAME);
    }
    catch (const std::exception& e) {
        ErrorManager::logError(e.what());
    }
}

// Handle main menu
void Game::baseHandleMenu() {
    Utils::cls();
    MenuAction action = menuManager.showMainMenu();

    switch (action) {
    case MenuAction::NEW_GAME:
        baseResetGame();
        screenGameManager.startFirstLevel(&p1, &p2, false);
        status = GameStatus::RUNNING;
        break;

    case MenuAction::LOAD_GAME:
        if (SaveManager::loadCompleteState(STATE_SAVE_FILE, p1, p2, levelTime, screenGameManager)) {
            screenGameManager.set_gameLoadSaved(true);

            Screen* currentScreen = screenGameManager.getCurrentScreen();
            if (currentScreen != nullptr) {
                currentScreen->setPlayers(&p1, &p2);
                currentScreen->reprintFullBoard(p1, p2);
            }
            status = GameStatus::RUNNING;
        }
        else {
            ErrorManager::logError("No save file found!");
            Utils::sleep(1000);
        }
        break;

    case MenuAction::INSTRUCTIONS:
        menuManager.showInstructions();
        break;

    case MenuAction::EXIT:
        baseHandleExitGame();
        break;

    default:
        break;
    }
}

// Handle game over screen
void Game::baseHandleGameOver() {
    Utils::cls();
    int totalScore = p1.getScore() + p2.getScore();
    std::string message = "GAME_OVER TOTAL_SCORE: " + std::to_string(totalScore);
    menuManager.showGameOver(totalScore);
    recordManager.logEvent(gameTick, message);
    status = GameStatus::MENU;
}

// Handle exit game
void Game::baseHandleExitGame() {
    menuManager.showExitGame();
    int totalScore = p1.getScore() + p2.getScore();
    std::string message = "GAME_OVER TOTAL_SCORE: " + std::to_string(totalScore);
    recordManager.logEvent(gameTick+1, message);
    status = GameStatus::EXIT;
}
// Load next level
void Game::baseLoadNextLevel() {
    Utils::cls();
    Utils::printTextFile(GAME_NEXT_LEV_FILE_NAME);
    Utils::sleep(1000);

    screenGameManager.moveToNextLevel();

    // Check if game is finished FIRST
    if (screenGameManager.isGameFinished()) {
        Utils::cls();
        Utils::printTextFile(GAME_WIN_FILE_NAME);
        Utils::sleep(500);
        status = GameStatus::GAME_OVER;
        return;
    }

    // Game not finished - move to next level
    Screen* nextScreen = screenGameManager.getCurrentScreen();
    if (nextScreen != nullptr) {
        int totalScore = p1.getScore() + p2.getScore();
        std::string message = "Level Up to " + std::to_string(screenGameManager.getCurrentLevelNumber()) + " TOTAL_SCORE: " + std::to_string(totalScore);
        recordManager.logEvent(gameTick, message);

        // Set players at entrance position
        Point enterPos = nextScreen->getNextRoomEnterPos();
        p1.setPosition(enterPos);
        p2.setPosition(enterPos);

        nextScreen->setPlayers(&p1, &p2);
        p1.resetLevelState();
        p2.resetLevelState();
        nextScreen->reprintFullBoard(p1, p2);
    }
}



// Load previous level
void Game::baseLoadPrevLevel() {
    // Can't go back from level 1
    if (screenGameManager.getCurrentLevelNumber() <= 1) {
        p1.setGoToPrevRoom(false);
        p2.setGoToPrevRoom(false);
        return;
    }

    Utils::cls();
    Utils::printTextFile(GAME_PREV_LEV_FILE_NAME);
    Utils::sleep(500);

    screenGameManager.moveToPrevLevel();

    Screen* prevScreen = screenGameManager.getCurrentScreen();
    if (prevScreen != nullptr) {
        int totalScore = p1.getScore() + p2.getScore();
        std::string message = "Level Back to " + std::to_string(screenGameManager.getCurrentLevelNumber()) + " TOTAL_SCORE: " + std::to_string(totalScore);
        recordManager.logEvent(gameTick, message);
        Point exitPos = prevScreen->getRoomExitPos();
        if (prevScreen->inBoard(exitPos)) {
            p1.setPosition(exitPos);
            p2.setPosition(exitPos);
            p1.setGoToPrevRoom(false);
            p2.setGoToPrevRoom(false);
            p1.resetLevelState();
            p2.resetLevelState();
            prevScreen->reprintFullBoard(p1, p2);
        }
    }
}

// Reset game to initial state
void Game::baseResetGame() {
    // Reset game variables
    gameTick = 0;
    levelTime = 0.0f;
    enemyMoveCounter = 0;

    // Reset player 1
    p1.setLives(INITIAL_LIVES);
    p1.setScore(0);
    p1.setKeys(0);
    p1.setFinished(false);
    p1.set_HasSword(false);
    p1.set_HasTorch(false);
    p1.set_HasBomb(false);

    // Reset player 2
    p2.setLives(INITIAL_LIVES);
    p2.setScore(0);
    p2.setKeys(0);
    p2.setFinished(false);
    p2.set_HasSword(false);
    p2.set_HasTorch(false);
    p2.set_HasBomb(false);

    // Reload levels
    screenGameManager.loadAllGameLevels(&p1, &p2);
    entityManager.clear();
    recordManager.logEvent(0, "Game Reset");
}

// Run one game tick
bool Game::baseRunTick() {
    onTickBegin();

    if (status != GameStatus::RUNNING) {
        return false;
    }

    // Get current screen
    Screen* currentScreenPtr = screenGameManager.getCurrentScreen();
    if (currentScreenPtr == nullptr) {
        status = GameStatus::EXIT;
        return false;
    }
    Screen& currentScreen = *currentScreenPtr;

    // Update time
    levelTime += static_cast<float>(GAME_SPEED_MS) / 1000.0f;

    // Check if level is complete
    if (p1.isFinished() && p2.isFinished()) {
        loadNextLevel();
        return true;
    }

    // Check if going back to previous level
    if (p1.getGoToPrevRoom() && p2.getGoToPrevRoom()) {
        loadPrevLevel();
        return true;
    }

    // Check for game over
    if (p1.getLives() <= 0 || p2.getLives() <= 0) {
        status = GameStatus::GAME_OVER;
    }

    // Save previous positions and lives
    Point prevPosP1 = p1.getPosition();
    Point prevPosP2 = p2.getPosition();
    int prevLivesP1 = p1.getLives();
    int prevLivesP2 = p2.getLives();
    bool p1FinishedBefore = p1.isFinished();
    bool p2FinishedBefore = p2.isFinished();

    // Handle input
    inputManager.handleInput(gameTick, p1, p2, entityManager, currentScreen,
        status, screenGameManager, levelTime);

    // Move players
    physicsManager.movePlayer(p1, p2, currentScreen, p1.getDirection());
    physicsManager.movePlayer(p2, p1, currentScreen, p2.getDirection());


    // Handle momentum
    if (p1.hasMomentum()) {
        physicsManager.movePlayer(p1, p2, currentScreen, p1.getMomentumDir());
    }
    if (p2.hasMomentum()) {
        physicsManager.movePlayer(p2, p1, currentScreen, p2.getMomentumDir());
    }

    // Update players
    p1.update(GAME_SPEED_MS);
    p2.update(GAME_SPEED_MS);

    // Update screen if players moved
    if (p1.getPosition() != prevPosP1 ) {
        currentScreen.movePlayer(prevPosP1, p1.getPosition(), p1);
    }
    if (p2.getPosition() != prevPosP2 ) {
        currentScreen.movePlayer(prevPosP2, p2.getPosition(), p2);
    }

    // Update bombs
    entityManager.updateBombs(currentScreen, p1, p2, (float)GAME_SPEED_MS);

    // Move enemies every 2 ticks
    enemyMoveCounter++;
    if (enemyMoveCounter >= 2) {
        entityManager.moveEnemies(currentScreen, p1, p2);
        enemyMoveCounter = 0;
    }

    // Log life changes
    if (p1.getLives() < prevLivesP1) {recordManager.logEvent(gameTick, "LOST_LIFE P1");}
    if (p1.getLives() > prevLivesP1) {recordManager.logEvent(gameTick, "GAIN_LIFE P1");}
    if (p1.isFinished() && !p1FinishedBefore) { recordManager.logEvent(gameTick, "EXIT_LEVEL P1"); }



    if (p2.getLives() < prevLivesP2) {recordManager.logEvent(gameTick, "LOST_LIFE P2");}
    if (p2.getLives() > prevLivesP2) {recordManager.logEvent(gameTick, "GAIN_LIFE P2");}
	if (p2.isFinished() && !p2FinishedBefore) { recordManager.logEvent(gameTick, "EXIT_LEVEL P2"); }

    // Update UI
    if (shouldRenderUI()) {
        currentScreen.updateStats(p1, p2, screenGameManager.getCurrentLevelNumber(),
            levelTime, status);
    }

    // Render and delay
    render();
    handleFrameDelay();

    onTickEnd();
    gameTick++;

    return true;
}