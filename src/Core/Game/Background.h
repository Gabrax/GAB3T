#include "../Backend/Shader.h"
#include "../Backend/Util.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Background {
    Background() : BackgroundTexture(0) {}

    void BindAndLoad()
    {
        glGenVertexArrays(1, &BackgroundVAO);
        glGenBuffers(1, &BackgroundVBO);
        glGenBuffers(1, &BackgroundEBO);
        glBindVertexArray(BackgroundVAO);
        glBindBuffer(GL_ARRAY_BUFFER, BackgroundVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(BackgroundVertices), BackgroundVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BackgroundEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(BackgroundIndices), BackgroundIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        BackgroundTexture = Util::loadTexture("res/textures/protoBackground.png");
    }

    void Render()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, BackgroundTexture);

        BackgroundShader.Use();
        BackgroundShader.setMat4("projection", Projection);
        BackgroundShader.setInt("texture1", 0);
        glBindVertexArray(BackgroundVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

private:

    float viewportWidth = 1920.0f;
    float viewportHeight = 1080.0f;
    float aspectRatio = viewportWidth / viewportHeight;
    glm::mat4 Projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);


    Shader& BackgroundShader = Util::g_shaders.basic;
    unsigned int BackgroundTexture;
    unsigned int BackgroundVAO, BackgroundVBO, BackgroundEBO;
    float BackgroundVertices[20] = { // Define array size explicitly
    // Positions           // Texture Coordinates
    -3.00f, -3.00f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
     3.00f, -3.00f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
     3.00f,  3.00f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
    -3.00f,  3.00f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int BackgroundIndices[6] = {  // Define array size explicitly
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
};



