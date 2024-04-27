#pragma once
#include <iostream>
#include <array>


#include "Shader.h"
#include "Utility.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>


struct TextBox {
    TextBox() : TextBoxShader("../MapShader.vert", "../MapShader.frag"), TextBoxTexture(0) {}

    void BindAndLoad() {
        glGenVertexArrays(1, &TextVAO);
        glGenBuffers(1, &TextVBO);
        glGenBuffers(1, &TextEBO);
        glBindVertexArray(TextVAO);
        glBindBuffer(GL_ARRAY_BUFFER, TextVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(MapVertices), MapVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TextEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MapIndices), MapIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        TextBoxTexture = loadTexture("../tictactoe.png");
    }

    void Render() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextBoxTexture);

        TextBoxShader.Use();
        TextBoxShader.setMat4("projection", MapProjection);
        TextBoxShader.setInt("texture1", 0);
        glBindVertexArray(TextVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

private:
    float viewportWidth = 1920.0f;
    float viewportHeight = 1080.0f;
    float aspectRatio = viewportWidth / viewportHeight;
    glm::mat4 MapProjection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);


    Shader TextBoxShader;
    unsigned int TextBoxTexture;
    unsigned int TextVAO, TextVBO, TextEBO;
    float MapVertices[20] = { // Define array size explicitly
    // Positions           // Texture Coordinates
    -1.0f,  1.0f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
     1.0f,  1.0f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
     1.0f,  4.0f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
    -1.0f,  4.0f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
};
    unsigned int MapIndices[6] = {  // Define array size explicitly
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
};
