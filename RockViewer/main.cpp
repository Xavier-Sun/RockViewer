#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "Camera.h"
#include "Time.h"
#include "Input.h"
#include "Resources.h"

int main(int argc, char* argv[])
{
	printf_s("程序已从%s启动。\n", *argv);

	if (!Window::GetInstance().Init(1600, 900, "ROCK VIEWER 1.0"))
	{
		return -1;
	}

	Input::Init();
	Camera::GetInstance().Init();

	Resources::GetInstance().LoadShaderFrom("D:/Shader/VertexShader.vert", "D:/Shader/FragmentShader.frag");
	Resources::GetInstance().LoadModelFrom("D:/Model/nanosuit.obj");

	while (Window::GetInstance().IsActive())
	{
		Time::Update();
		Input::Update();
		Camera::GetInstance().Update();
		Window::GetInstance().Update();
	}

	return 0;
}