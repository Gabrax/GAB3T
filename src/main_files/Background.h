#include <iostream>
#include <vector>
#include <gif_lib.h>
#include "Shader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct GifRenderer {
    GifFileType* gif;
    Shader gifShader;
    std::vector<unsigned int> gifTextures;
    unsigned int gifVAO, gifVBO, gifEBO;
    int currentFrame;
    bool render;

    GifRenderer() : gifShader("../MapShader.vert", "../MapShader.frag"), currentFrame(0), render(true), gif(nullptr), gifVAO(0), gifVBO(0), gifEBO(0) {}

    ~GifRenderer() {
        close();
    }

    void BindAndLoad(const char* filename) {
        int error_code;
        gif = DGifOpenFileName(filename, &error_code);
        if (!gif) {
            std::cerr << "Error opening GIF file: " << GifErrorString(error_code) << std::endl;
            return;
        }

        if (DGifSlurp(gif) != GIF_OK) {
            std::cerr << "Error reading GIF file." << std::endl;
            DGifCloseFile(gif, nullptr);
            gif = nullptr;
            return;
        }

        if (gif->ImageCount == 0) {
            std::cerr << "No frames found in GIF." << std::endl;
            DGifCloseFile(gif, nullptr);
            gif = nullptr;
            return;
        }

        // // Clear any existing frames
        // gifTextures.clear();

        // // Reserve memory for textures
        // gifTextures.reserve(gif->ImageCount);

        // Initialize vertex array object and buffers
        glGenVertexArrays(1, &gifVAO);
        glGenBuffers(1, &gifVBO);
        glGenBuffers(1, &gifEBO);

        glBindVertexArray(gifVAO);
        glBindBuffer(GL_ARRAY_BUFFER, gifVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gifVertices), gifVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gifEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gifIndices), gifIndices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // // Loop through each frame
        // for (int i = 0; i < gif->ImageCount; ++i) {
        //     GifImageDesc& desc = gif->SavedImages[i].ImageDesc;

        //     if (desc.Width <= 0 || desc.Height <= 0) {
        //         std::cerr << "Invalid frame size in GIF." << std::endl;
        //         continue;
        //     }

        //     // Calculate expected buffer size
        //     int expectedSize = desc.Width * desc.Height * 3; // 3 bytes per pixel assuming RGB

        //     // Check if RasterBits is nullptr
        //     if (gif->SavedImages[i].RasterBits == nullptr) {
        //         std::cerr << "RasterBits is null for frame " << i << std::endl;
        //         continue;
        //     }

        //     // Generate and bind a new texture object
        //     unsigned int gifTexture;
        //     glGenTextures(1, &gifTexture);
        //     glBindTexture(GL_TEXTURE_2D, gifTexture);

        //     // Set texture parameters
        //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        //     // Upload buffer data to texture
        //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, desc.Width, desc.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, gif->SavedImages[i].RasterBits);

        //     // Check for OpenGL errors
        //     GLenum error = glGetError();
        //     if (error != GL_NO_ERROR) {
        //         std::cerr << "OpenGL error: " << error << std::endl;
        //         // Cleanup resources
        //         glDeleteTextures(1, &gifTexture);
        //         close();
        //         return;
        //     }

        //     gifTextures.push_back(gifTexture);
        // }

        // // Close GIF file
        // DGifCloseFile(gif, nullptr);
        // gif = nullptr;
    }

    void renderNextFrame() {
        if (render) {
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(gifVAO);
            gifShader.Use();

            glBindTexture(GL_TEXTURE_2D, gifTextures[currentFrame]);
            gifShader.setInt("texture1", 0);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Move to the next frame
            currentFrame = (currentFrame + 1) % gifTextures.size();
        }
    }

    void close() {
        // Add your cleanup code here if needed
    }

private:
    const float gifVertices[20] = {
        // Positions           // Texture Coordinates
        -1.0f, -1.8f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
        1.0f, -1.8f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
        1.0f,  1.8f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
        -1.0f,  1.8f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };

    const unsigned int gifIndices[6] = {
        0, 1, 2,
        2, 3, 0
    };
};
