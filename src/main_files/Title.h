#pragma once
#include <iostream>
#include <array>


#include "Shader.h"
#include "Utility.h"
#include "Input.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>


struct StartScreen {
    StartScreen() : StartScreenShader("../TitleShader.vert", "../TitleShader.frag"), render(true), StartScreenTexture(0) {}
    
    ~StartScreen(){
        // Delete buffers and perform any cleanup operations
        glDeleteBuffers(1, &StartVBO);
        glDeleteBuffers(1, &StartEBO);
        glDeleteVertexArrays(1, &StartVAO);

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

        StartScreenTexture = loadTexture("../title.png");
    }
    
    // flag for rendering
    bool render;

    void Render() {
        if(render){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, StartScreenTexture);

            StartScreenShader.Use();
            StartScreenShader.setMat4("projection", BoxProjection);
            StartScreenShader.setInt("texture1", 0);
            StartScreenShader.setFloat("time",glfwGetTime());
            glBindVertexArray(StartVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

    void Delete() {
        if (Input::KeyPressed(GAB_KEY_ENTER)){
           this->~StartScreen();
		}
    }

private:

    float viewportWidth = 1920.0f;
    float viewportHeight = 1080.0f;
    float aspectRatio = viewportWidth / viewportHeight;
    glm::mat4 BoxProjection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);


    Shader StartScreenShader;
    unsigned int StartScreenTexture;
    unsigned int StartVAO, StartVBO, StartEBO;
    float TitleVertices[20] = { // Define array size explicitly
    // Positions           // Texture Coordinates
    -1.0f, -0.5f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
     1.0f, -0.5f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
     1.0f,  0.5f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
    -1.0f,  0.5f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
};
    unsigned int TitleIndices[6] = {  // Define array size explicitly
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
};
