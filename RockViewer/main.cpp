#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "Camera.h"
#include "Time.h"
#include "Input.h"
#include "Resources.h"
#include "Window.h"

int main(int argc, char* argv[])
{
	printf_s("程序已从%s启动。\n", *argv);

	if (!Window::GetInstance().Init(1600, 900, "ROCK VIEWER 1.0"))
	{
		return -1;
	}

	Input::Init();
	Camera::GetInstance().Init();

	Resources::GetInstance().LoadShaderFrom("Blinn Phong", "Shaders/BlinnPhong.vert", "Shaders/BlinnPhong.frag");
	Resources::GetInstance().LoadShaderFrom("Phong", "Shaders/Phong.vert", "Shaders/Phong.frag");
	Resources::GetInstance().LoadModelFrom("Models/SampleModel.obj");

	while (Window::GetInstance().IsActive())
	{
		Time::Update();
		Input::Update();
		Camera::GetInstance().Update();
		Window::GetInstance().Update();
	}

	return 0;
}