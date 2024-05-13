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


