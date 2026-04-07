#include "GameFactory.h"
#include "ErrorManager.h"

int main(int argc, char* argv[]) {
    try {
        // Create the game instance by mode
        auto game = GameFactory::createGame(argc, argv);

        // Run the game
        game->run();
    }
    catch (const std::exception& e) {
        ErrorManager::logError("Critical Error: " + std::string(e.what()));
        return 1;
    }
    return 0;
}