#pragma once
#include "Utils.h"
#include "Constants.h"
#include "Screen.h"
#include "ColorManager.h"

#include <conio.h>
#include <iostream>

class UIdisplayManager : public Screen {
public:
	UIdisplayManager() = default;
	~UIdisplayManager() = default;

	// Display a message screen from a text file
    void show_UI_Screen_Msg_display(const std::string& filename ,Color col = Color::WHITE) {
        Utils::cls();
        Utils::setTextColor(col);
		Utils::printTextFile(filename);
        (void)_getch();
    }
   
 

    // Display game over screen with final score
    void showGameOver(int finalScore){
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


    // Display exit game screen
    void showExitGame() {
        Utils::cls();
        Utils::setTextColor(Color::WHITE);
        Utils::printTextFile(EXIT_GAME_FILE_NAME);
        Utils::setTextColor(Color::WHITE);
        (void)_getch();
    }

    // Display game saved screen
    void showGameSavedMasg(){
        Utils::cls();
        Utils::setTextColor(Color::WHITE);
        Utils::printTextFile(GAME_SAVED_SUCCESSFULLY_FILE_NAME);
        Utils::setTextColor(Color::WHITE);
        (void)_getch();
	}

};

