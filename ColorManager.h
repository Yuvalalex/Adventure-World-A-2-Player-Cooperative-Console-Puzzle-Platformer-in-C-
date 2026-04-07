#pragma once
#include "Constants.h"

class ColorManager {
public:
    // Get default color for object type
    static Color getColor(ObjectType type);

    // Dynamic color handlers for interactive objects
    static Color getBombColor(bool isTicking, int flickerState);
    static Color getSwitchColor(bool isOn);
    static Color getExitColor(bool isOn);
    static Color getPlayerColor(int playerId);
};