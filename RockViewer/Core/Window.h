#ifndef WINDOW_H
#define WINDOW_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Graphics/FrameBuffer.h>

class Window
{
public:
	FrameBuffer frameBuffer;

	static Window& GetInstance()
	{
		static Window instance;
		return instance;
	}

	GLFWwindow* GetGLFWwindow() const { return glfwWindow; }

	bool IsActive() const { return !glfwWindowShouldClose(glfwWindow); }
	void SetActive(bool active) const { glfwSetWindowShouldClose(glfwWindow, !active); }

	bool Init();
	void Update();

	~Window();

private:
	GLFWwindow* glfwWindow;

	std::vector<size_t> materialIndexVector;

	bool canShowSceneWindow = true;
	bool canShowSettingsWindow = true;
	bool canShowResourcesWindow = true;

	Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void ShowSceneWindow();
	void ShowSettingsWindow();
	void ShowResourcesWindow();
};

#endif