#include "Game/Game.h"

////////////////////////////////////////////////////////////////////
//   BIGTODO: Make this standalone application and not Librarie   //
////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
    Game game;

    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}
