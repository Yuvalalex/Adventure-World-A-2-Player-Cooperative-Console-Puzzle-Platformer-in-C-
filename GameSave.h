#pragma once
#include "Game.h"
#include <string>

class GameSave : public Game {
	std::string filename;
public:
	explicit GameSave(const std::string& file);
	~GameSave() override = default;

	bool run() override;

protected:
	void onInit() override;
	void render() override;
	void handleFrameDelay() override;
	void onGameEnd() override;

	void commonInit() override;
	void handleMenu() override;
	void handleGameOver() override;
	void handleExitGame() override;
	void loadNextLevel() override;
	void loadPrevLevel() override;
	void resetGame() override;
};
