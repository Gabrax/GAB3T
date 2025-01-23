#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

namespace Window
{
	void Init(int width, int height);
	void ShowFPS();
	void ProcessInput();
  void BeginFrame();
	void EndFrame();
	void Cleanup();
	bool WindowIsOpen();
	int GetWindowWidth();
	int GetWindowHeight();
	int GetCursorX();
	int GetCursorY();
	void DisableCursor();
	void HideCursor();
	void ShowCursor();
	GLFWwindow* GetWindowPtr();
	int GetCursorScreenX();
	int GetCursorScreenY();
	bool WindowHasFocus();
	bool WindowHasNotBeenForceClosed();
	void ForceCloseWindow();
  float getAspectRatio();
	enum WindowMode { WINDOWED, FULLSCREEN };
	enum RenderMode { WIREFRAME, NORMAL };
	void CreateWindow(WindowMode windowMode);
	void SetWindowMode(WindowMode windowMode);
	void SetRenderMode(RenderMode renderMode);
	void ToggleFullscreen();
	void ToggleWireframe();
	int GetScrollWheelYOffset();
	void ResetScrollWheelYOffset();
	void processInput(GLFWwindow* window);
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void window_focus_callback(GLFWwindow* window, int focused);
	// camera
  inline Camera _camera(glm::vec3(0.0f,0.0f,15.0f));
  inline float _lastX = static_cast<float>(GetWindowWidth()) / 2.0f;
  inline float _lastY = static_cast<float>(GetWindowHeight()) / 2.0f;
  inline bool _firstMouse = true;
}
