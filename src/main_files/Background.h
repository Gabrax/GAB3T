#pragma once
#include <iostream>
#include <vector>
#include <gif_lib.h>
#include "Shader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct GifRenderer {
    GifFileType* gif;
    Shader gifShader;
    unsigned int gifTexture;
    unsigned int gifVAO, gifVBO, gifEBO;
    std::vector<std::vector<unsigned char>> frames; // Store frame data directly
    int currentFrame;
    bool render;

    GifRenderer() : gifShader("../MapShader.vert", "../MapShader.frag"), currentFrame(0), render(true), gif(nullptr), gifTexture(0), gifVAO(0), gifVBO(0), gifEBO(0) {}

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
            close();
            return;
        }

        if (gif->ImageCount == 0) {
            std::cerr << "No frames found in GIF." << std::endl;
            close();
            return;
        }

        glGenTextures(1, &gifTexture);
        glBindTexture(GL_TEXTURE_2D, gifTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        for (int i = 0; i < gif->ImageCount; ++i) {
        GifImageDesc& desc = gif->SavedImages[i].ImageDesc;

        if (desc.Width <= 0 || desc.Height <= 0) {
            std::cerr << "Invalid frame size in GIF." << std::endl;
            close();
            return;
        }

        // Calculate expected buffer size
        int expectedSize = desc.Width * desc.Height * 4; // 4 bytes per pixel assuming RGBA

        // Check if RasterBits is nullptr
        if (gif->SavedImages[i].RasterBits == nullptr) {
            std::cerr << "RasterBits is null for frame " << i << std::endl;
            close();
            return;
        }

        // Ensure that the index i is within bounds of the frames vector
        if (i >= frames.size()) {
            std::cerr << "Index out of bounds for frames vector." << std::endl;
            close();
            return;
        }

         // Check if RasterBits is nullptr
        if (gif->SavedImages[i].RasterBits == nullptr) {
            std::cerr << "RasterBits is null for frame " << i << std::endl;
            close();
            return;
        }

        // Create buffer and copy raster bits
        std::vector<unsigned char>& buffer = frames[i];
        buffer.resize(expectedSize); // Resize the buffer to match the expected size
        memcpy(buffer.data(), gif->SavedImages[i].RasterBits, expectedSize);

        // Upload buffer data to texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, desc.Width, desc.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
    }


        glGenVertexArrays(1, &gifVAO);
        glGenBuffers(1, &gifVBO);
        glGenBuffers(1, &gifEBO);

        glBindVertexArray(gifVAO);
        glBindBuffer(GL_ARRAY_BUFFER, gifVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, gifVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gifEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, gifIndices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void renderNextFrame() {
        if(render){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gifTexture);
            gifShader.Use();
            gifShader.setInt("texture1", 0);
            glBindVertexArray(gifVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

    void close() {
        // if (gif) {
        //     for (unsigned char frame : frames)
        //         delete[] frame;
        //     DGifCloseFile(gif, nullptr);
        //     gif = nullptr;
        // }
        // frames.clear();

        // if (gifTexture) {
        //     glDeleteTextures(1, &gifTexture);
        //     gifTexture = 0;
        // }

        // if (gifVAO) {
        //     glDeleteVertexArrays(1, &gifVAO);
        //     gifVAO = 0;
        // }

        // if (gifVBO) {
        //     glDeleteBuffers(1, &gifVBO);
        //     gifVBO = 0;
        // }

        // if (gifEBO) {
        //     glDeleteBuffers(1, &gifEBO);
        //     gifEBO = 0;
        // }
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
