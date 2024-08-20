#pragma once
#include <iostream>
#include <array>

#include "Shader.h"
#include "Utility.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>


struct Player {
    Player() : PlayerShader("Resources/Player.vert", "Resources/Player.frag"), PlayerTexture(0) {}

    float getNewX() const {
        return displacementX;
    }
    float getNewY() const {
        return displacementY;
    }

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

        PlayerTexture = loadTexture("Resources/new_O.png");
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

    void UpdatePositionFromBorder(float newX, float newY) {
    // Check if the new position is within the bounds
    if (newX >= -1.3f && newX <= 0.65f && newY >= -2.34f && newY <= 1.17f) {
        // Calculate the displacement from the current position
        displacementX = newX - PlayerVertices[0];
        displacementY = newY - PlayerVertices[1];

        // Update position of the player vertices
        for (int i = 0; i < 20; i += 5) {
            PlayerVertices[i] += displacementX;
            PlayerVertices[i + 1] += displacementY;
        }

        // Update the new position
        PlayerVertices[0] = newX;
        PlayerVertices[1] = newY;

        // Bind and update the vertex buffer object
        glBindBuffer(GL_ARRAY_BUFFER, PlayerVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(PlayerVertices), PlayerVertices, GL_STATIC_DRAW);
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


    Shader PlayerShader;
    unsigned int PlayerTexture;
    unsigned int PlayerVAO, PlayerVBO, PlayerEBO;
    float PlayerVertices[20] = { // Define array size explicitly
        // Positions           // Texture Coordinates
        -0.28f, -0.52f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
        0.28f, -0.52f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
        0.28f,  0.49f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
        -0.28f,  0.49f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int PlayerIndices[6] = {  // Define array size explicitly
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
};

struct InstantiatePlayer {

    InstantiatePlayer() = default;
    ~InstantiatePlayer() = default;
    // Add a new image to render
    void AddPlayer(const Player& player) {
        players.push_back(player);
    }

    // Render all the added images
    void RenderAllPlayers() {
        for (auto& player : players) {
            player.Render();
        }
    }

    void Delete(){
        this->~InstantiatePlayer();
    }

    void clearPlayer(){
        players.clear();
    }

private:
    std::vector<Player> players;
};
