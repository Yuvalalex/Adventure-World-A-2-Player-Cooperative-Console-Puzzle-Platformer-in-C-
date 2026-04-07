#pragma once
#include "InputStrategy.h"
#include "RecordManager.h"
#include "Game.h"
#include <conio.h>

// 1. Keyboard input strategy
class KeyboardStrategy : public InputStrategy {
public:
    bool fetchKey(char& outKey, long gameTick) override {
        if (_kbhit()) {
            outKey = _getch();
            return true;
        }
        return false;
    }
};

// 2. Save strategy
class SaveStrategy : public InputStrategy {
private:
    RecordManager& recordManager;
public:
    SaveStrategy(RecordManager& rm) : recordManager(rm) {}

    bool fetchKey(char& outKey, long gameTick) override {
        if (_kbhit()) {
            outKey = _getch();

            recordManager.recordStep(gameTick, outKey);
            return true;
        }
        return false;
    }
};

// 3. Load strategy
class LoadStrategy : public InputStrategy {
private:
    RecordManager& recordManager;
public:
    LoadStrategy(RecordManager& rm) : recordManager(rm) {}

    bool fetchKey(char& outKey, long gameTick) override {
        // Replay is driven only by the recorded file; ignore any real user input.
        if (recordManager.readStep(gameTick, outKey)) {
            return true;  
        }

        return false;
    }

    bool shouldExit() const override {
        Game* g = Game::getInstance();
        if (!g) return recordManager.isFinished();
        const long lastTick = recordManager.getLastRecordedTick();
        if (lastTick < 0) return true;

        // Keep running empty ticks until we reach the END recorded tick.
        return g->getGameTick() >= lastTick;
    }
};
