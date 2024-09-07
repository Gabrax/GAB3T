#include <iostream>
#include <vector>

#include "engine.h"
#include "window.h"
#include "Input.h"
#include "GameLogic.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include "stb_image.h"

void Engine::Run(){

    Window::Init((int)(1920 * 0.75f),(int)(1920 * 0.75f));
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    Logic::Game game;
    game.initialize();


    while (Window::WindowIsOpen() && Window::WindowHasNotBeenForceClosed())
    {  
        //Window::ShowFPS();

        glClear(GL_COLOR_BUFFER_BIT);

        
        game.update();


        if (Input::KeyPressed(GAB_KEY_F))
		{
			Window::ToggleFullscreen();   
		}
		if (Input::KeyPressed(GAB_KEY_H))
		{
			Window::ToggleWireframe();
        }

        Window::ProcessInput();
        Input::Update();
        Window::SwapBuffersPollEvents();
    }

    //DEBUG 
    //game.printMapCoord();
    //game.printTuple();
    //game.printBoard(Logic::board);
}




