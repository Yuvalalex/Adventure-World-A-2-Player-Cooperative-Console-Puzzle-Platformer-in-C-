#pragma once

#include <memory>
#include <string>
#include "Constants.h"
#include "GameSimple.h"
#include "GameSave.h"
#include "GameLoad.h"

// Factory class to create the right game mode
class GameFactory {
public:
    // Create a game based on command line arguments
    static std::unique_ptr<Game> createGame(int argc, char* argv[]) {
        GameMode mode = GameMode::SIMPLE;
        std::string recordFile = "game_log";
        std::string expectedFile = "";
        bool sawLoad = false;
        bool sawSilent = false;

        // Parse command line arguments
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];

            if (arg == "-save" || arg == "--save") {
                mode = GameMode::SAVE;
            }
            else if (arg == "-load" || arg == "--load") {
                // Silent mode overrides load mode
                if (mode != GameMode::SILENT) {
                    mode = GameMode::LOAD;
                }
                sawLoad = true;
            }
            else if (arg == "-silent" || arg == "--silent") {
                mode = GameMode::SILENT;
                sawSilent = true;
            }
            else if (arg.find(".txt") != std::string::npos) {
                // This is a file name
                if (mode == GameMode::SILENT && recordFile != "game_log") {
                    // Second file is expected results
                    expectedFile = arg;
                }
                else {
                    recordFile = arg;
                }
            }
            else if (arg == "pause") {
                // Ignore pause argument
                continue;
            }
            else {
                // Assume it's a file name without .txt
                recordFile = arg;
            }
        }

        // Silent mode requires load
        if (sawSilent && !sawLoad) {
            sawLoad = true;
        }

        // Create the appropriate game mode
        switch (mode) {
        case GameMode::SAVE:
        {
            auto game = std::make_unique<GameSave>(recordFile);
            game->setGameMode(GameMode::SAVE);
            return game;
        }

        case GameMode::SILENT:
        {
            // Silent mode uses GameLoad with silent flag
            auto game = std::make_unique<GameLoad>(recordFile, true);
            game->setGameMode(GameMode::SILENT);
            return game;
        }

        case GameMode::LOAD:
        {
            auto game = std::make_unique<GameLoad>(recordFile);
            game->setGameMode(GameMode::LOAD);
            return game;
        }

        case GameMode::SIMPLE:
        default:
        {
            auto game = std::make_unique<GameSimple>();
            game->setGameMode(GameMode::SIMPLE);
            return game;
        }
        }
    }
};