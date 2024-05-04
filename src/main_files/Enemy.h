#pragma once
#include <iostream>
#include <array>

#include "Shader.h"
#include "Utility.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>


struct Enemy {
    Enemy() : EnemyShader("../Player.vert", "../Player.frag"), EnemyTexture(0) {}

    float getNewX() const {
        return displacementX;
    }
    float getNewY() const {
        return displacementY;
    }

    void BindAndLoad() {
        glGenVertexArrays(1, &EnemyVAO);
        glGenBuffers(1, &EnemyVBO);
        glGenBuffers(1, &EnemyEBO);
        glBindVertexArray(EnemyVAO);
        glBindBuffer(GL_ARRAY_BUFFER, EnemyVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(EnemyVertices), EnemyVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EnemyEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(EnemyIndices), EnemyIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        EnemyTexture = loadTexture("../Ximage.png");
    }

    void Render() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, EnemyTexture);

        EnemyShader.Use();
        EnemyShader.setMat4("projection", MapProjection);
        EnemyShader.setInt("texture1", 0);

        //Translate the model to the specified position
        //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, 0.0f));
        //EnemyShader.setMat4("model", model);

        glBindVertexArray(EnemyVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void UpdatePositionFromBorder(float newX, float newY) {
    // Check if the new position is within the bounds
    if (newX >= -1.3f && newX <= 0.65f && newY >= -2.34f && newY <= 1.17f) {
            // Calculate the displacement from the current position
            displacementX = newX - EnemyVertices[0];
            displacementY = newY - EnemyVertices[1];

            // Update position of the Enemy vertices
            for (int i = 0; i < 20; i += 5) {
                EnemyVertices[i] += displacementX;
                EnemyVertices[i + 1] += displacementY;
            }

            // Update the new position
            EnemyVertices[0] = newX;
            EnemyVertices[1] = newY;

            // Bind and update the vertex buffer object
            glBindBuffer(GL_ARRAY_BUFFER, EnemyVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(EnemyVertices), EnemyVertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

private:

    float displacementX;
    float displacementY;

    float viewportWidth = 1920.0f;
    float viewportHeight = 1080.0f;
    float aspectRatio = viewportWidth / viewportHeight;
    glm::mat4 MapProjection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);


    Shader EnemyShader;
    unsigned int EnemyTexture;
    unsigned int EnemyVAO, EnemyVBO, EnemyEBO;
    float EnemyVertices[20] = { // Define array size explicitly
        // Positions           // Texture Coordinates
        -0.35f, -0.64f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
         0.35f, -0.64f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
         0.35f,  0.62f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
        -0.35f,  0.62f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int EnemyIndices[6] = {  // Define array size explicitly
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
};

struct InstantiateEnemy {

    InstantiateEnemy() = default;
    ~InstantiateEnemy() = default;

    // Add a new image to render
    void AddEnemy(const Enemy& Enemy) {
        Enemys.push_back(Enemy);
    }

    // Render all the added images
    void RenderAllEnemys() {
        for (auto& Enemy : Enemys) {
            Enemy.Render();
        }
    }

    void Delete(){
        this->~InstantiateEnemy();
    }

private:
    std::vector<Enemy> Enemys;
};
