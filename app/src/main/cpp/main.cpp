#include "game/Game.h"

void android_main(struct android_app* app) {
    Game* game = new Game(app);
    game->startGameLoop();
    delete game;
}
