#pragma once
#include <iostream>
#include <array>


#include "Shader.h"
#include "Utility.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>


struct modeBorder {
    modeBorder() : BorderShader("../MapShader.vert", "../MapShader.frag"), modeBorderTexture(0), render(true) {}

    ~modeBorder(){
        glDeleteBuffers(1, &BorderVBO);
        glDeleteBuffers(1, &BorderEBO);
        glDeleteVertexArrays(1, &BorderVAO);

        render = false;
    }

    float getNewX() const {
        return newX;
    }
    float getNewY() const {
        return newY;
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

        modeBorderTexture = loadTexture("../borderProto.png");
    }

    // flag for rendering
    bool render;

    void Render() {
        if(render){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, modeBorderTexture);

            BorderShader.Use();
            BorderShader.setMat4("projection", Projection);
            BorderShader.setInt("texture1", 0);
            glBindVertexArray(BorderVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }


    void UpdatePosition(float x, float y) {
        // Calculate the new position
        newY = BorderVertices[1] + y;

        // Check if the new position is within the bounds
        if (newY >= -1.1f && newY <= 0.4f) {
            // Update position of the border vertices
            for (int i = 0; i < 20; i += 5) {
                BorderVertices[i + 1] += y;
            }

            // Bind and update the vertex buffer object
            glBindBuffer(GL_ARRAY_BUFFER, BorderVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(BorderVertices), BorderVertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void Delete(){
        this->~modeBorder();
    }

private:
    float newX;
    float newY;

    float viewportWidth = 1920.0f;
    float viewportHeight = 1080.0f;
    float aspectRatio = viewportWidth / viewportHeight;
    glm::mat4 Projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);


    Shader BorderShader;
    unsigned int modeBorderTexture;
    unsigned int BorderVAO, BorderVBO, BorderEBO;
    float BorderVertices[20] = { // Define array size explicitly
        // Positions           // Texture Coordinates
        -0.6f, -1.1f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
         0.6f, -1.1f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
         0.6f, -0.4f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
        -0.6f, -0.4f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int BorderIndices[6] = {  // Define array size explicitly
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
};
