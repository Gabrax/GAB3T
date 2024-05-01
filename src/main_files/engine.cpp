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
#include "Enemy.h"

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
    InstantiateEnemy InstEnem;
    

    GameMap.BindAndLoad();
    Title.BindAndLoad();
    Border.BindAndLoad();

    bool renderPlayer = false;
    bool updatePlayer = true;
    bool isPlayer1Turn = true;

    std::cout << "Your turn" << '\n';

    while (Window::WindowIsOpen() && Window::WindowHasNotBeenForceClosed())
    {  
        Window::ShowFPS();


        Title.Render();
        Title.Delete();

        if(!Title.render){
            GameMap.Render();
            Border.Render();

            Player player1;
            player1.BindAndLoad();

            Enemy enemy;
            enemy.BindAndLoad();

            // Check whose turn it is and update the corresponding player
        if (isPlayer1Turn) {
            if (Input::KeyPressed(GAB_KEY_UP)) {
                Border.UpdatePosition(0.0f, 1.17f); // Move image up
            } else if (Input::KeyPressed(GAB_KEY_DOWN)) {
                Border.UpdatePosition(0.0f, -1.17f); // Move image down
            } else if (Input::KeyPressed(GAB_KEY_LEFT)) {
                Border.UpdatePosition(-0.65f, 0.0f); // Move image left
            } else if (Input::KeyPressed(GAB_KEY_RIGHT)) {
                Border.UpdatePosition(0.65f, 0.0f); // Move image right
            } else if (Input::KeyPressed(GAB_KEY_X)) {
                // Add player 1's move to the map coordinates and render the player
                mapCoord.emplace_back(Border.getNewX(), Border.getNewY());
                player1.UpdatePositionFromBorder(Border.getNewX(), Border.getNewY());
                InstPlay.AddPlayer(player1);
                
                // Toggle to player 2's turn
                isPlayer1Turn = false;
                std::cout << "Player2 turn" << '\n';
            }
        } else {
            // Similar logic for player 2's turn
            if (Input::KeyPressed(GAB_KEY_UP)) {
                Border.UpdatePosition(0.0f, 1.17f); // Move image up
            } else if (Input::KeyPressed(GAB_KEY_DOWN)) {
                Border.UpdatePosition(0.0f, -1.17f); // Move image down
            } else if (Input::KeyPressed(GAB_KEY_LEFT)) {
                Border.UpdatePosition(-0.65f, 0.0f); // Move image left
            } else if (Input::KeyPressed(GAB_KEY_RIGHT)) {
                Border.UpdatePosition(0.65f, 0.0f); // Move image right
            } else if (Input::KeyPressed(GAB_KEY_X)) {
                // Add player 2's move to the map coordinates and render the player
                mapCoord.emplace_back(Border.getNewX(), Border.getNewY());
                enemy.UpdatePositionFromBorder(Border.getNewX(), Border.getNewY());
                InstEnem.AddEnemy(enemy);
                
                // Toggle to player 1's turn
                isPlayer1Turn = true;
                std::cout << "Your turn" << '\n';
                }
            }
        }


        //std::cout << Border.getNewX() << " " << Border.getNewY() << '\n';


        if (Input::KeyPressed(GAB_KEY_F))
		{
			Window::ToggleFullscreen();   
		}
		if (Input::KeyPressed(GAB_KEY_H))
		{
			Window::ToggleWireframe();
        }

        InstPlay.RenderAllPlayers();
        InstEnem.RenderAllEnemys();
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


