#pragma once
#include <iostream>
#include <array>


#include "Shader.h"
#include "Utility.h"
#include "Input.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>


struct ChooseMode {
    ChooseMode() : ChooseModeShader("../MapShader.vert", "../MapShader.frag"),
                    PressEnterShader("../MapShader.vert", "../MapShader.frag") ,render(true) {}

    ~ChooseMode(){
        glDeleteBuffers(1, &StartVBO);
        glDeleteBuffers(1, &StartEBO);
        glDeleteVertexArrays(1, &StartVAO);

        glDeleteBuffers(1, &EnterVBO);
        glDeleteBuffers(1, &EnterEBO);
        glDeleteVertexArrays(1, &EnterVAO);

        render = false;
    }
    

    void BindAndLoad() {
        glGenVertexArrays(1, &StartVAO);
        glGenBuffers(1, &StartVBO);
        glGenBuffers(1, &StartEBO);
        glBindVertexArray(StartVAO);
        glBindBuffer(GL_ARRAY_BUFFER, StartVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TitleVertices), TitleVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, StartEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TitleIndices), TitleIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        ChooseModeTexture = loadTexture("../pvp.png");

        glGenVertexArrays(1, &EnterVAO);
        glGenBuffers(1, &EnterVBO);
        glGenBuffers(1, &EnterEBO);
        glBindVertexArray(EnterVAO);
        glBindBuffer(GL_ARRAY_BUFFER, EnterVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(EnterVertices), EnterVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EnterEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(EnterIndices), EnterIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        PressEnterTexture = loadTexture("../pve.png");
    }
    
    // flag for rendering
    bool render;

    void Render() {
        if(render){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ChooseModeTexture);

            ChooseModeShader.Use();
            ChooseModeShader.setMat4("projection", projection);
            ChooseModeShader.setInt("texture1", 0);
            ChooseModeShader.setFloat("time",glfwGetTime());
            glBindVertexArray(StartVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, PressEnterTexture);

            PressEnterShader.Use();
            PressEnterShader.setMat4("projection", projection);
            PressEnterShader.setInt("texture1", 1);
            glBindVertexArray(EnterVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

    void Delete() {
        if (Input::KeyPressed(GAB_KEY_ENTER)){
           this->~ChooseMode();
		}
    }

private:

    float viewportWidth = 1920.0f;
    float viewportHeight = 1080.0f;
    float aspectRatio = viewportWidth / viewportHeight;
    glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);


    Shader ChooseModeShader;
    unsigned int ChooseModeTexture;
    unsigned int StartVAO, StartVBO, StartEBO;
    float TitleVertices[20] = { 
        // Positions           // Texture Coordinates
        -0.5f, -1.0f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
         0.5f, -1.0f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
         0.5f, -0.5f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
        -0.5f, -0.5f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int TitleIndices[6] = {  
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };

    Shader PressEnterShader;
    unsigned int PressEnterTexture;
    unsigned int EnterVAO, EnterVBO, EnterEBO;

    float EnterVertices[20] = { 
        // Positions           // Texture Coordinates
        -0.5f, 0.0f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
         0.5f, 0.0f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
         0.5f, 0.5f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
        -0.5f, 0.5f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int EnterIndices[6] = {  
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
};
