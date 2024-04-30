#include <iostream>
#include <vector>
#include <utility>

#include "engine.h"
#include "window.h"
#include "camera.h"
#include "Shader.h"
#include "Input.h"
#include "GameMap.h"
#include "Title.h"
#include "Select.h"
#include "Player.h"
#include "Instantiate.h"

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

std::vector<std::pair<float,float>> mapCoord;

void Engine::Run(){

    Window::Init(1920 * 1.5f, 1080 * 1.5f);
    glfwSwapInterval(0);
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    GameMap GameMap;
    StartScreen Title;
    SelectBorder Border;
    InstantiatePlayer InstPlay;

    GameMap.BindAndLoad();
    Title.BindAndLoad();
    Border.BindAndLoad();

    bool renderPlayer = false;
    bool updatePlayer = true;

    while (Window::WindowIsOpen() && Window::WindowHasNotBeenForceClosed())
    {  
        Window::ShowFPS();

        Player Player;
        Player.BindAndLoad();

        Title.Render();
        Title.Delete();

        if(!Title.render){
            GameMap.Render();
            Border.Render();


            if (Input::KeyPressed(GAB_KEY_UP)) {
                Border.UpdatePosition(0.0f, 1.17f); // Move image up
                    //Player.UpdatePosition(0.0f, 1.17f); // Move image up
                
            }
            if (Input::KeyPressed(GAB_KEY_DOWN)) {
                Border.UpdatePosition(0.0f, -1.17f); // Move image down
                    //Player.UpdatePosition(0.0f, -1.17f); // Move image down
    
            }
            if (Input::KeyPressed(GAB_KEY_LEFT)) {
                Border.UpdatePosition(-0.65f, 0.0f); // Move image left
                    //Player.UpdatePosition(-0.65f, 0.0f); // Move image left
                
            }
            if (Input::KeyPressed(GAB_KEY_RIGHT)) {
                Border.UpdatePosition(0.65f, 0.0f); // Move image right
                    //Player.UpdatePosition(0.65f, 0.0f); // Move image left
                
            }
            if (Input::KeyPressed(GAB_KEY_X)) {
                //renderPlayer = true;
                //updatePlayer = false;
                mapCoord.emplace_back(Player.getNewX(),Player.getNewY());
                Player.UpdatePosition(Border.getNewX(),Border.getNewY());
                InstPlay.AddPlayer(Player);
            }
        }


        std::cout << Player.getNewX() << " " << Player.getNewY() << '\n';


        if (Input::KeyPressed(GAB_KEY_F))
		{
			Window::ToggleFullscreen();   
		}
		if (Input::KeyPressed(GAB_KEY_H))
		{
			Window::ToggleWireframe();
        }

        InstPlay.RenderAllPlayers();
        Window::ProcessInput();
        Input::Update();
        Window::SwapBuffersPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
    }

    std::cout << "-----------" << '\n';
    for(const auto& i : mapCoord){
        std::cout << i.first << " " << i.second << '\n';
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


