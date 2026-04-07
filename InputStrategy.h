#pragma once

class InputStrategy {
public:
    virtual ~InputStrategy() = default;

    virtual bool fetchKey(char& outKey, long gameTick) = 0;

    // Check if we should exit the game loop
    virtual bool shouldExit() const { return false; }
};
