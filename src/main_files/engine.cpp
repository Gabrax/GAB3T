#include <iostream>

#include "engine.h"
#include "window.h"
#include "camera.h"
#include "Shader.h"

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

unsigned int loadTexture(const char* path);

unsigned int VBO, VAO, EBO;








void Engine::Run(){

    Window::Init(1920 * 1.5f, 1080 * 1.5f);
    glfwSwapInterval(0);

    float Crosshairvertices[] = {
        // Positions           // Texture Coordinates
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,    1.0f, 0.0f,
        0.5f,  0.5f, 0.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f
    };
    unsigned int Crosshairindices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };

    Shader _Crosshair("Shaders/Crosshair.vert", "Shaders/Crosshair.frag");

        unsigned int CrossVAO, CrossVBO, CrossEBO;
        glGenVertexArrays(1, &CrossVAO);
        glGenBuffers(1, &CrossVBO);
        glGenBuffers(1, &CrossEBO);
        glBindVertexArray(CrossVAO);
        glBindBuffer(GL_ARRAY_BUFFER, CrossVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Crosshairvertices), Crosshairvertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CrossEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Crosshairindices), Crosshairindices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        // glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    unsigned int GameMap;
    GameMap = loadTexture("C:/Users/gab-o/Documents/GitHub/OpenGL_project/src/main_files/tictactoe.png");
    stbi_set_flip_vertically_on_load(true);

    while (Window::WindowIsOpen() && Window::WindowHasNotBeenForceClosed())
    {  
        Window::ShowFPS();
        Window::ProcessInput();


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GameMap);

        glm::mat4 UIprojection = glm::ortho(0.0f, static_cast<float>(800.0f), 0.0f, static_cast<float>(600.0f));

        _Crosshair.Use();
        _Crosshair.setMat4("projection", UIprojection);
        _Crosshair.setInt("texture1", 0);
        glBindVertexArray(CrossVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



        Window::SwapBuffersPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

unsigned int loadTexture(const char* path){
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
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

// void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
// {
//     camera.ProcessMouseScroll(static_cast<float>(yoffset));
// }

