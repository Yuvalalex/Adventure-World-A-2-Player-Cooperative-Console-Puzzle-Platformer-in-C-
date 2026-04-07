#pragma once
#include "BaseGameObject.h"

class Wall : public BaseGameObject {
public:
    // Walls are non-passable obstacles
    Wall(Point loc)
        : BaseGameObject(ObjectType::WALL, CHAR_WALL, loc, true, false) {}
};