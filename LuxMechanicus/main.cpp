#include "Utils\GLIncludes.h"
#include <iostream>

#include "Game\Game.h"

int main()
{
    Game* game = new Game();
    
    game->RenderLoop();

    delete game;
    return 0;
}

