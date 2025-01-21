#include "engine.h"
#include "Core/Backend/window.h"
#include "Core/Game/GameLogic.h"
#include "Core/Backend/Util.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void Engine::Run(){

    Window::Init((int)(1920 * 0.75f),(int)(1920 * 0.75f));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Util::BakeShaders();
    Game game;


    while (Window::WindowIsOpen() && Window::WindowHasNotBeenForceClosed())
    {  
        Window::BeginFrame();
        //Window::ShowFPS();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        game.update();

        Window::EndFrame();
    }

    //DEBUG 
    //game.printMapCoord();
    //game.printTuple();
    //game.printBoard(Logic::board);
}
