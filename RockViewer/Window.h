#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	static Window& GetInstance()
	{
		static Window instance;
		return instance;
	}

	struct GUI
	{
		bool canShowSceneWindow = true;
		bool canShowMessageWindow = true;
	};

	struct FrameBuffer
	{
		unsigned int FBO = 0u;
		unsigned int renderTexture = 0u;
		unsigned int RBO = 0u;
		int width = 800;
		int height = 600;
	};

	GUI gui;

	FrameBuffer frameBuffer;

	~Window();

	GLFWwindow* GetGLFWwindow() { return glfwWindow; }

	bool IsActive() { return !glfwWindowShouldClose(glfwWindow); }
	void SetActive(bool active) { glfwSetWindowShouldClose(glfwWindow, !active); }

	bool Init(int width, int height, const char* title);
	void Update();

private:
	GLFWwindow* glfwWindow;

	Window();
	Window(const Window&) = delete;

	void ShowSceneWindow();
	void ShowMessageWindow();
};

#endif