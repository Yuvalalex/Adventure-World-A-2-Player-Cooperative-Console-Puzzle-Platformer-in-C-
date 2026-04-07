#pragma once
#include <string>
#include <vector>
#include "Point.h" 

// Forward declarations
class Player;
class Screen;

struct Riddle {
    std::string question;
    std::string answer;
};

struct RiddleResult {
    bool solved;
    bool answered;
    int playerId;
    std::string question;
    std::string answer;
    bool correct;

    RiddleResult()
        : solved(false), answered(false), playerId(0), correct(false) {
    }
};

class RiddleManager {
private:
    static std::vector<Riddle> riddles;

public:
    // Load riddles from file
    static bool loadRiddles(const std::string& filename);

    // Calculate ID based on location 
    static int getRiddleIdForLocation(Point p);

    //  Accept specific ID for execution 
    static RiddleResult handleRiddleInteraction(Player& player, Player& otherPlayer,
        Screen& screen, int riddleId);
};
