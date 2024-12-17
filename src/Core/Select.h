#pragma once
#include <iostream>
#include <array>

#include "Shader.h"
#include "Util.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


struct SelectBorder {
    SelectBorder() : BorderShader("res/shaders/MapShader.vert", "res/shaders/MapShader.frag"), SelectBorderTexture(0) {}

    float getNewX() const {
        return BorderVertices[0];
    }
    float getNewY() const {
        return BorderVertices[1];
    }

    void BindAndLoad() {
        glGenVertexArrays(1, &BorderVAO);
        glGenBuffers(1, &BorderVBO);
        glGenBuffers(1, &BorderEBO);
        glBindVertexArray(BorderVAO);
        glBindBuffer(GL_ARRAY_BUFFER, BorderVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(BorderVertices), BorderVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BorderEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(BorderIndices), BorderIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        SelectBorderTexture = Util::loadTexture("res/textures/new_select_border.png");
    }

    void Render() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, SelectBorderTexture);

        BorderShader.Use();
        BorderShader.setMat4("projection", Projection);
        BorderShader.setInt("texture1", 0);
        glBindVertexArray(BorderVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


    void UpdatePosition(float x, float y) {
        // Calculate the new position
        newX = BorderVertices[0] + x;
        newY = BorderVertices[1] + y;

        // Check if the new position is within the bounds
        if (newX >= -0.87f && newX <= 0.31f && newY >= -1.59f && newY <= 1.17f) {
            // Update position of the border vertices
            for (int i = 0; i < 20; i += 5) {
                BorderVertices[i] += x;
                BorderVertices[i + 1] += y;
            }

            glBindBuffer(GL_ARRAY_BUFFER, BorderVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(BorderVertices), BorderVertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void ResetPosition() {
        // Reset the position to initial values
        newX = 0.0f; // Set newX to initial X position
        newY = 0.0f; // Set newY to initial Y position

        // Update position of the border vertices to initial values
        for (int i = 0; i < 20; i += 5) {
            BorderVertices[i] = InitialBorderVertices[i]; // Set X position of vertex
            BorderVertices[i + 1] = InitialBorderVertices[i + 1]; // Set Y position of vertex
        }

        // Bind and update the vertex buffer object
        glBindBuffer(GL_ARRAY_BUFFER, BorderVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(BorderVertices), BorderVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    float newX;
    float newY;

    float viewportWidth = 1920.0f;
    float viewportHeight = 1080.0f;
    float aspectRatio = viewportWidth / viewportHeight;
    glm::mat4 Projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);


    Shader BorderShader;
    unsigned int SelectBorderTexture;
    unsigned int BorderVAO, BorderVBO, BorderEBO;
    float BorderVertices[20] = { // Define array size explicitly
    // Positions           // Texture Coordinates
    -0.28f, -0.52f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
     0.28f, -0.52f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
     0.28f,  0.49f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
    -0.28f,  0.49f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int BorderIndices[6] = {  // Define array size explicitly
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
    float InitialBorderVertices[20] = {
        // Positions           // Texture Coordinates
        -0.28f, -0.52f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
         0.28f, -0.52f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
         0.28f,  0.49f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
        -0.28f,  0.49f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
};
