#include "./Game/Game.h"

int main(int argc, char *argv[])
{

    Game game;
    game.Initialize();

    // game loop
    game.Run();

    game.Destroy();

    return 0;
}
