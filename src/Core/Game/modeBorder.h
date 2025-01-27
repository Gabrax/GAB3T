#pragma once
#include <iostream>
#include <array>


#include "../Backend/Shader.h"
#include "../Backend/Util.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include "../Backend/window.h"
#include "../Backend/Util.hpp"

struct modeBorder
{
    modeBorder() = default; 

    ~modeBorder(){
        glDeleteBuffers(1, &BorderVBO);
        glDeleteBuffers(1, &BorderEBO);
        glDeleteVertexArrays(1, &BorderVAO);
    }

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

        modeBorderTexture = Util::loadTexture("res/textures/borderProto.png");
    }

    void Render(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f)) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, modeBorderTexture);

        _shader.Use();
        _shader.setVec3("viewPos", this->_camera.Position);
        glm::mat4 projection = glm::perspective(glm::radians(this->_camera.Zoom), Window::getAspectRatio(), 0.001f, 2000.0f);
        _shader.setMat4("projection", projection);
        _shader.setMat4("view", this->_camera.GetViewMatrix());
        // Model transformation
        Util::Transform transform;
        transform.position = position;
        transform.rotation = rotation;
        transform.scale = scale;

        _shader.setMat4("model", transform.to_mat4());

        _shader.setInt("texture1", 2);
        glBindVertexArray(BorderVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

    Shader& _shader = Util::g_shaders.basic;
    Camera& _camera = Window::_camera;
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
