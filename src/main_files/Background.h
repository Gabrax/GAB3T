#include <iostream>
#include <vector>
#include "cgif.h"
#include "Shader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


struct GIFCreator {
    CGIF* pGIF;
    CGIF_Config gConfig;
    std::vector<uint8_t> pImageData;
    std::vector<uint8_t> aPalette;
    uint16_t numColors;
    int numFrames;

    GIFCreator(const char* path, uint16_t width, uint16_t height)
        : pGIF(nullptr), numColors(3), numFrames(12) {
        initGIFConfig(&gConfig, path, width, height, aPalette, numColors);
        pGIF = cgif_newgif(&gConfig);
        pImageData.resize(width * height);
        aPalette = {0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF}; // Red, Green, Blue
    }

    ~GIFCreator() {
        cgif_close(pGIF);
    }

    void initGIFConfig(CGIF_Config* pConfig, const char* path, uint16_t width, uint16_t height, std::vector<uint8_t>& pPalette, uint16_t numColors) {
        memset(pConfig, 0, sizeof(CGIF_Config));
        pConfig->width = width;
        pConfig->height = height;
        pConfig->pGlobalPalette = pPalette.data();
        pConfig->numGlobalPaletteEntries = numColors;
        pConfig->path = path;
        pConfig->attrFlags = CGIF_ATTR_IS_ANIMATED;
    }

    void initFrameConfig(CGIF_FrameConfig* pConfig, uint8_t* pImageData, uint16_t delay) {
        memset(pConfig, 0, sizeof(CGIF_FrameConfig));
        pConfig->delay = delay;
        pConfig->pImageData = pImageData;
    }

    void createFrames() {
        for (int f = 0; f < numFrames; ++f) {
            for (int i = 0; i < (gConfig.width * gConfig.height); ++i) {
                pImageData[i] = static_cast<uint8_t>((f + i % gConfig.width ) / 4 % numColors); // moving stripe pattern (4 pixels per stripe)
            }
            CGIF_FrameConfig fConfig;
            initFrameConfig(&fConfig, pImageData.data(), 10);
            cgif_addframe(pGIF, &fConfig);
        }
    }
};

struct Background {
    Background() : BackgroundShader("../MapShader.vert", "../MapShader.frag"), BackgroundTexture(0) {}

    void BindAndLoad() {
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

        BackgroundTexture = loadTexture("../protoBackground.png");
    }

    void Render() {
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


    Shader BackgroundShader;
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


