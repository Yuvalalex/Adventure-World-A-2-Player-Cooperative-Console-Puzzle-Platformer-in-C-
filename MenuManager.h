#pragma once
#include "Utils.h"
#include "Constants.h"
#include "Screen.h"

#include <conio.h>
#include <iostream>

class MenuManager  : public Screen {
public:
    MenuManager() = default;
    ~MenuManager() = default; 
public:
    // Display main menu and return user's choice
    MenuAction showMainMenu();

    // Display game instructions
    void showInstructions();

    // Display pause menu and return user's choice
    PauseAction showPauseMenu();

    // Display game over screen with final score
    void showGameOver(int finalScore);

    // Display exit game screen
    void showExitGame();

    // Display game saved screen
    void showGameSavedMasg();

};