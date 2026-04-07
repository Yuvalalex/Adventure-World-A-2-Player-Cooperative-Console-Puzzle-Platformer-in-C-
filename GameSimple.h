#pragma once
#include "Game.h"

class GameSimple : public Game {
public:
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
};
