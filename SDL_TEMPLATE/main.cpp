#include "Game.h"
#include "GameFPS.h"

int main(int argc, char* argv[]) {
    Game* game = new Game;

    game->init();
    game->startMenu();

    GameFPS::initCalculateFPS();
    while (game->isRunning()) {
        GameFPS::initLimitFPS();

        game->input();
        game->update();
        game->render();

        GameFPS::calculateFPS();
        GameFPS::limitFPS();
    }

    game->close();
    return 0;
}
