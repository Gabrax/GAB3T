#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "Shader.h"

#include <iostream>

namespace Util {

  struct Shaders 
  {
    Shader model;
    Shader basic;
    Shader light;
    Shader bloom_downsample; 
    Shader bloom_upsample;
    Shader bloom_final;   
    Shader skybox;
  };
  inline Shaders g_shaders;

  inline void BakeShaders()
  {
    g_shaders.model.Load("res/shaders/model.glsl");
    g_shaders.basic.Load("res/shaders/2D.glsl");
    g_shaders.light.Load("res/shaders/light.glsl");
    g_shaders.skybox.Load("res/shaders/skybox.glsl");
    g_shaders.bloom_downsample.Load("res/shaders/bloom_downsample.glsl");
    g_shaders.bloom_upsample.Load("res/shaders/bloom_upsample.glsl");
    g_shaders.bloom_final.Load("res/shaders/bloom_final.glsl");
  }

  inline void HotReloadShaders()
  {
    std::cout << "HotReloading shaders...\n";
    BakeShaders();
  }

  struct Transform
  {
      glm::vec3 position = glm::vec3(0);
      glm::vec3 rotation = glm::vec3(0);
      glm::vec3 scale = glm::vec3(1);
      glm::mat4 to_mat4() {
          glm::mat4 m = glm::translate(glm::mat4(1), position);
          m *= glm::mat4_cast(glm::quat(rotation));
          m = glm::scale(m, scale);
          return m;
      };
      glm::vec3 to_forward_vector() {
          glm::quat q = glm::quat(rotation);
          return glm::normalize(q * glm::vec3(0.0f, 0.0f, 1.0f));
      }
      glm::vec3 to_right_vector() {
          glm::quat q = glm::quat(rotation);
          return glm::normalize(q * glm::vec3(1.0f, 0.0f, 0.0f));
      }
  };

  inline unsigned int loadTexture(const char* path){
      unsigned int textureID;
      glGenTextures(1, &textureID);

      int width, height, nrComponents;
      unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
      if (data)
      {
          GLenum format;
          if (nrComponents == 1)
              format = GL_RED;
          else if (nrComponents == 3)
              format = GL_RGB;
          else if (nrComponents == 4)
              format = GL_RGBA;

          glBindTexture(GL_TEXTURE_2D, textureID);
          glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
          glGenerateMipmap(GL_TEXTURE_2D);

          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

          stbi_image_free(data);
      }
      else
      {
          std::cout << "Texture failed to load at path: " << path << std::endl;
          stbi_image_free(data);
      }

      return textureID;
  }

  inline unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false) {
      std::string filename = directory + '/' + path;

      unsigned int textureID;
      glGenTextures(1, &textureID);

      int width, height, nrComponents;
      unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
      if (data) {
          GLenum format = (nrComponents == 1) ? GL_RED : (nrComponents == 3) ? GL_RGB : GL_RGBA;
          glBindTexture(GL_TEXTURE_2D, textureID);
          glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
          glGenerateMipmap(GL_TEXTURE_2D);

          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

          stbi_image_free(data);
      } else {
          std::cerr << "Texture failed to load at path: " << path << std::endl;
          stbi_image_free(data);
      }

      return textureID;
  }

  inline unsigned int TextureFromMemory(const unsigned char* data, size_t dataSize, bool gamma = false) {
      int width, height, nrComponents;

      // Load the texture from memory
      unsigned char* imageData = stbi_load_from_memory(data, static_cast<int>(dataSize), &width, &height, &nrComponents, 0);
      if (imageData) {
          GLenum format = (nrComponents == 1) ? GL_RED : (nrComponents == 3) ? GL_RGB : GL_RGBA;

          unsigned int textureID;
          glGenTextures(1, &textureID);
          glBindTexture(GL_TEXTURE_2D, textureID);
          glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
          glGenerateMipmap(GL_TEXTURE_2D);

          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

          stbi_image_free(imageData);
          return textureID;
      } else {
          std::cerr << "Texture failed to load from embedded data" << std::endl;
          return 0;
      }
  }
}

