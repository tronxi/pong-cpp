#include "Game.h"

int main() {
    Game game = Game();
    const bool initialized = game.init();
    if (initialized) {
        game.run();
    }
    game.shutdown();
    return 0;
}
