#pragma once

#include "Game.h"
#include <string>

class GameLoad : public Game {
protected:
    std::string filename;
    bool isSilentMode;
    int sleepSpeed;

public:
    explicit GameLoad(const std::string& file, bool silentMode = false);
    virtual ~GameLoad() = default;

	bool run() override;

protected:
    void onInit() override;
    void render() override;
    void handleFrameDelay() override;

	void commonInit() override;
	void handleMenu() override;
	void handleGameOver() override;
	void handleExitGame() override;
	void loadNextLevel() override;
	void loadPrevLevel() override;
	void resetGame() override;

	bool shouldExitRunLoop() const override;
};