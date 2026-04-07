#include "Utils.h"
#include <fstream>
#include <conio.h>
#include <iostream>


bool Utils::isColorEnabled = true;
bool Utils::_isSilentMode = false;







void Utils::setTextColor(Color color) {
    if (_isSilentMode) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
		return;
    }
    else {
        if (!isColorEnabled) {
                color = Color::WHITE;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(color));
    }
    
}

void Utils::cls() {
    system("cls");
}

void Utils::hideCursor() {

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void Utils::gotoxy(int x, int y) {
	
    COORD coord;
    coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void Utils::sleep(int milliseconds) {
    if (_isSilentMode) return;
    Sleep(milliseconds);
}

void Utils::printTextFile(const std::string& filename) {
    if (_isSilentMode) return;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::cout << file.rdbuf();
    }
    else {
        std::cout << "Error: " << filename << " missing.\n";
    }
}

