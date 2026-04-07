#pragma once

// Forward declarations
class Screen;
class Player;

class EntityManager {
public:
    // Update enemy for all active enemies
    void moveEnemies(Screen& screen, Player& p1, Player& p2);

    // Update all active bombs (timers and explosions)
    void updateBombs(Screen& screen, Player& p1, Player& p2, float deltaTime);

    void clear() {}
};