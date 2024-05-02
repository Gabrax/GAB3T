#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"

// extern Camera camera;
// extern float deltaTime;
// extern float lastFrame;
// extern float lastX;
// extern float lastY;
// extern bool firstMouse;

inline Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
inline float deltaTime = 0.0f;
inline float lastFrame = 0.0f;
inline float lastX = 800 / 2.0f;
inline float lastY = 600 / 2.0f;
inline bool firstMouse = true;

namespace Engine
{
	void Run();
	inline void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}


