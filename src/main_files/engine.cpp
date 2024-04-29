#include <iostream>

#include "engine.h"
#include "window.h"
#include "camera.h"
#include "Shader.h"
#include "Input.h"
#include "GameMap.h"
#include "Title.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include "stb_image.h"


//camera properties
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;


void Engine::Run(){

    Window::Init(1920 * 1.5f, 1080 * 1.5f);
    glfwSwapInterval(0);
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    GameMap GameMap;
    StartScreen Title;

    GameMap.BindAndLoad();
    Title.BindAndLoad();


    while (Window::WindowIsOpen() && Window::WindowHasNotBeenForceClosed())
    {  
        Window::ShowFPS();


        Title.Render();
        Title.Delete();
        
        if(Title.render == false){
            GameMap.Render();
        }
        

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
        glClear(GL_COLOR_BUFFER_BIT);
    }
}



void Engine::mouse_callback(GLFWwindow* window, double xposIn, double yposIn){


    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


