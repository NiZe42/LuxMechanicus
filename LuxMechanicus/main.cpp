#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Game.h"

int main()
{
    Game* game = new Game();
    
    game->RenderLoop();

    delete game;
    return 0;
}

