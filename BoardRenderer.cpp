#include "BoardRenderer.h"
#include "Utils.h"
#include <iostream>

void BoardRenderer::drawObject(BaseGameObject* obj) {
    if (!obj) {
        return;
    }

    Utils::gotoxy(obj->getLoc().getX(), obj->getLoc().getY());
    Utils::setTextColor(obj->getColor());
    std::cout << obj->getRepr();
    Utils::setTextColor(Color::WHITE);
}

void BoardRenderer::clearObject(Point position) {
    Utils::gotoxy(position.getX(), position.getY());
    std::cout << ' ';
}

void BoardRenderer::moveObject(Point from, Point to, char symbol,
    Color color, char backgroundChar) {
    // Clear previous position
    Utils::gotoxy(from.getX(), from.getY());
    Utils::setTextColor(Color::WHITE);
    std::cout << backgroundChar;

    // Draw at new position
    Utils::gotoxy(to.getX(), to.getY());
    Utils::setTextColor(color);
    std::cout << symbol;
    Utils::setTextColor(Color::WHITE);
}