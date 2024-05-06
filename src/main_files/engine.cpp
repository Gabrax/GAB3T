#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "Shader.h"
#include "Input.h"
#include "GameMap.h"
#include "Title.h"
#include "Select.h"
#include "Player.h"
#include "Enemy.h"
#include "engine.h"
#include "window.h"
#include "Input.h"
#include "GameLogic.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include "stb_image.h"


void Engine::Run(){

    Window::Init(1920 * 1.5f, 1080 * 1.5f);
    glfwSwapInterval(0);
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    Logic::Game game;
    game.initialize();


    while (Window::WindowIsOpen() && Window::WindowHasNotBeenForceClosed())
    {  
        Window::ShowFPS();

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

    //game.printMapCoord();
    //game.printTuple();
    //game.printBoard(Logic::board);
}




