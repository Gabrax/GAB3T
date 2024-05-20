#pragma once
#include <iostream>
#include <array>
#include <unordered_map>


#include "Shader.h"
#include "Utility.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>



struct WinBox {
    WinBox() : BoxShader("../MapShader.vert", "../MapShader.frag"), WinBoxTexture(0), render(false) {
        textures["playerOwins"] = loadTexture("../playerOwins.png");
        textures["playerXwins"] = loadTexture("../playerXwins.png");
        textures["draw"] = loadTexture("../draw.png");
    }

    bool Pwin = false;
    bool Ewin = false;
    bool Draw = false;

    void BindAndLoad() {
        glGenVertexArrays(1, &BoxVAO);
        glGenBuffers(1, &BoxVBO);
        glGenBuffers(1, &BoxEBO);
        glBindVertexArray(BoxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, BoxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(MapVertices), MapVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BoxEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MapIndices), MapIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    bool render;

    void Render() {
        if(render){
            GLuint textureToUse = 0; // Default texture ID

            if (Pwin) {
            textureToUse = textures["playerOwins"];
            }
            if (Ewin) {
                textureToUse = textures["playerXwins"];
            }
            if (Draw) {
                textureToUse = textures["draw"];
            }

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureToUse);

            BoxShader.Use();
            BoxShader.setMat4("projection", MapProjection);
            BoxShader.setInt("texture1", 0);
            glBindVertexArray(BoxVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

private:
    float viewportWidth = 1920.0f;
    float viewportHeight = 1080.0f;
    float aspectRatio = viewportWidth / viewportHeight;
    glm::mat4 MapProjection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);
    std::unordered_map<std::string, GLuint> textures;



    Shader BoxShader;
    unsigned int WinBoxTexture;
    unsigned int BoxVAO, BoxVBO, BoxEBO;
    float MapVertices[20] = { // Define array size explicitly
            // Positions           // Texture Coordinates
        -1.0f,  2.0f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
         1.0f,  2.0f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
         1.0f,  2.5f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
        -1.0f,  2.5f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int MapIndices[6] = {  // Define array size explicitly
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
};
