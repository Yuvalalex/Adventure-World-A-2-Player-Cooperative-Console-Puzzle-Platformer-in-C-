#include "MenuManager.h"

MenuAction MenuManager::showMainMenu() {
    Utils::cls();
    Utils::setTextColor(Color::LIGHTCYAN);
    Utils::printTextFile(MAIN_MENU_FILE_NAME);

    char input = _getch();

    if (input == '1') return MenuAction::NEW_GAME;
    if (input == '2') return MenuAction::LOAD_GAME;
    if (input == '3') { Utils::toggleColor(); return MenuAction::NONE; }
    if (input == '8') return MenuAction::INSTRUCTIONS;
    if (input == '9') return MenuAction::EXIT;

    return MenuAction::NONE;
}

void MenuManager::showInstructions() {
    Utils::cls();
    Utils::printTextFile(GAME_USER_INSTRUCTIONS_FILE_NAME);
    (void)_getch();
}

PauseAction MenuManager::showPauseMenu() {
    int x = 23, y = 9;
    Utils::gotoxy(x, y);

    auto printLine = [&](const char* text) {
        Utils::gotoxy(x, y++);
        std::cout << text;
        };

    printLine("  ========================  ");
    printLine("        GAME PAUSED         ");
    printLine("  ========================  ");
    printLine("  Press (S) - To Save state ");
    printLine("  Press (H) - To Main Menu   ");
    printLine("  ========================  ");
    printLine("  Press (ESC) - To Continue  ");
    printLine("  ========================  ");

    while (true) {
        if (_kbhit()) {
            char c = _getch();

            if (c == 's' || c == 'S') {
                return PauseAction::SAVE;
            }
            if (c == 'h' || c == 'H') {
                return PauseAction::EXIT_TO_MENU;
            }
            if (c == 27) { // ESC key
                return PauseAction::CONTINUE;
            }
        }
    }
}

void MenuManager::showGameOver(int finalScore) {
    Utils::cls();
    Utils::setTextColor(Color::WHITE);
    Utils::printTextFile(GAME_OVER_FILE_NAME);

    // Display final score
    Utils::gotoxy(57, 15);
    Utils::setTextColor(Color::WHITE);

    if (finalScore != 0) {
        std::cout << finalScore;
    }

    Utils::setTextColor(Color::WHITE);
    (void)_getch();
}

void MenuManager::showExitGame() {
    Utils::cls();
    Utils::setTextColor(Color::WHITE);
    Utils::printTextFile(EXIT_GAME_FILE_NAME);
    Utils::setTextColor(Color::WHITE);
    (void)_getch();
}





void MenuManager::showGameSavedMasg() {
    Utils::cls();
    Utils::setTextColor(Color::WHITE);
    Utils::printTextFile(GAME_SAVED_SUCCESSFULLY_FILE_NAME);
    Utils::setTextColor(Color::WHITE);
    (void)_getch();
}