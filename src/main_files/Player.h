#pragma once
#include <iostream>
#include <array>

#include "Shader.h"
#include "Utility.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>


struct Player {
    Player() : PlayerShader("../Player.vert", "../Player.frag"), PlayerTexture(0) {}

    void BindAndLoad() {
        glGenVertexArrays(1, &PlayerVAO);
        glGenBuffers(1, &PlayerVBO);
        glGenBuffers(1, &PlayerEBO);
        glBindVertexArray(PlayerVAO);
        glBindBuffer(GL_ARRAY_BUFFER, PlayerVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(PlayerVertices), PlayerVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PlayerEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PlayerIndices), PlayerIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        PlayerTexture = loadTexture("../Oimage.png");
    }

    void Render() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, PlayerTexture);

        PlayerShader.Use();
        PlayerShader.setMat4("projection", MapProjection);
        PlayerShader.setInt("texture1", 0);

        //Translate the model to the specified position
        //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, 0.0f));
        //PlayerShader.setMat4("model", model);

        glBindVertexArray(PlayerVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void UpdatePosition(float x, float y) {
        // Calculate the new position
        newX = PlayerVertices[0] + x;
        newY = PlayerVertices[1] + y;

        // Check if the new position is within the bounds
        if (newX >= -1.3f && newX <= 0.65f && newY >= -2.34f && newY <= 1.17f) {
            // Update position of the border vertices
            for (int i = 0; i < 20; i += 5) {
                PlayerVertices[i] += x;
                PlayerVertices[i + 1] += y;
            }

            // Bind and update the vertex buffer object
            glBindBuffer(GL_ARRAY_BUFFER, PlayerVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(PlayerVertices), PlayerVertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
    

private:

    float newX;
    float newY;

    float viewportWidth = 1920.0f;
    float viewportHeight = 1080.0f;
    float aspectRatio = viewportWidth / viewportHeight;
    glm::mat4 MapProjection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);


    Shader PlayerShader;
    unsigned int PlayerTexture;
    unsigned int PlayerVAO, PlayerVBO, PlayerEBO;
    float PlayerVertices[20] = { // Define array size explicitly
        // Positions           // Texture Coordinates
        -0.35f, -0.64f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
         0.35f, -0.64f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
         0.35f,  0.62f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
        -0.35f,  0.62f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int PlayerIndices[6] = {  // Define array size explicitly
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
};
