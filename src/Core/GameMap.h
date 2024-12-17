#pragma once

#include "Shader.h"
#include "Util.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "glm/ext/matrix_clip_space.hpp"

struct GameMap {
    GameMap() : MapShader("res/shaders/MapShader.vert", "res/shaders/MapShader.frag"), GameMapTexture(0), render(true) {}

    ~GameMap(){
        glDeleteBuffers(1, &MapVBO);
        glDeleteBuffers(1, &MapEBO);
        glDeleteVertexArrays(1, &MapVAO);

        render = false;
    }

    void BindAndLoad() {
        glGenVertexArrays(1, &MapVAO);
        glGenBuffers(1, &MapVBO);
        glGenBuffers(1, &MapEBO);
        glBindVertexArray(MapVAO);
        glBindBuffer(GL_ARRAY_BUFFER, MapVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(MapVertices), MapVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MapEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MapIndices), MapIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        GameMapTexture = Util::loadTexture("res/textures/new_map.png");
    }

    bool render;

    void Render() {
        if(render){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, GameMapTexture);

            MapShader.Use();
            MapShader.setMat4("projection", MapProjection);
            MapShader.setInt("texture1", 0);
            glBindVertexArray(MapVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

    void Delete(){
        this->~GameMap();
    }

private:
    float viewportWidth = 1920.0f;
    float viewportHeight = 1080.0f;
    float aspectRatio = viewportWidth / viewportHeight;
    glm::mat4 MapProjection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);


    Shader MapShader;
    unsigned int GameMapTexture;
    unsigned int MapVAO, MapVBO, MapEBO;
    float MapVertices[20] = { // Define array size explicitly
    // Positions           // Texture Coordinates
    -1.0f, -1.8f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
     1.0f, -1.8f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
     1.0f,  1.8f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
    -1.0f,  1.8f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int MapIndices[6] = {  // Define array size explicitly
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
};
