#pragma once
#include "Point.h"
#include "Constants.h"
#include "BaseGameObject.h"

class BoardRenderer {
public:
    void drawObject(BaseGameObject* obj);
    void clearObject(Point position);
    void moveObject(Point from, Point to, char symbol, Color color, char backgroundChar);
};