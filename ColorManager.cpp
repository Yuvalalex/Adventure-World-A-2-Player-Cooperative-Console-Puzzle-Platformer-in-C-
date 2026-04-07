#include "ColorManager.h"

Color ColorManager::getColor(ObjectType type) {
    switch (type) {
  
    case ObjectType::WALL:      return Color::DARKGREY;   
    case ObjectType::DOOR:      return Color::BROWN;     
    case ObjectType::EXIT:      return Color::WHITE;      
    case ObjectType::OBSTACLE:  return Color::LIGHTGREY;  

       
    case ObjectType::ENEMY:     return Color::LIGHTRED;   
    case ObjectType::MINE:      return Color::RED;
    case ObjectType::BOMB:      return Color::YELLOW;

    case ObjectType::EXTRA_LIFE:return Color::LIGHTGREEN; 
    case ObjectType::KEY:       return Color::YELLOW;     
    case ObjectType::TORCH:     return Color::BLUE;

    case ObjectType::RIDDLE:    return Color::LIGHTMAGENTA; 
    case ObjectType::TELEPORT:  return Color::CYAN;       
    case ObjectType::SPRING:    return Color::LIGHTCYAN;  
    case ObjectType::SWITCH:    return Color::BLUE;       

    default:                    return Color::WHITE;
    }
}

Color ColorManager::getBombColor(bool isTicking, int flickerState) {
    if (!isTicking) {
        return Color::RED;
    }
    // Flicker effect between Yellow and Red when ticking
    return (flickerState % 5 == 0) ? Color::YELLOW : Color::RED;
}

Color ColorManager::getSwitchColor(bool isOn) {
    return isOn ? Color::LIGHTGREEN : Color::RED;
}

Color ColorManager::getExitColor(bool isOn) {
    return isOn ? Color::YELLOW : Color::YELLOW;
}

Color ColorManager::getPlayerColor(int playerId) {
    if (playerId == 1) return Color::LIGHTCYAN;
    if (playerId == 2) return Color::LIGHTMAGENTA;
    return Color::WHITE;
}