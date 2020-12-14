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

	if (!Window::GetInstance().Init())
	{
		return -1;
	}

	Input::Init();
	Camera::GetInstance().Init();

	Resources::GetInstance().LoadShaderFromFile("Blinn Phong", "Shaders/BlinnPhong.vert", "Shaders/BlinnPhong.frag");
	Resources::GetInstance().LoadShaderFromFile("Phong", "Shaders/Phong.vert", "Shaders/Phong.frag");
	Resources::GetInstance().LoadShaderFromFile("Standard", "Shaders/Standard.vert", "Shaders/Standard.frag");
	Resources::GetInstance().LoadModelFromFile("Models/SampleModel.obj");
	Resources::GetInstance().LoadTextureFromFile("Textures/arm_dif.png");
	Resources::GetInstance().LoadTextureFromFile("Textures/body_dif.png");
	Resources::GetInstance().LoadTextureFromFile("Textures/glass_dif.png");
	Resources::GetInstance().LoadTextureFromFile("Textures/hand_dif.png");
	Resources::GetInstance().LoadTextureFromFile("Textures/helmet_diff.png");
	Resources::GetInstance().LoadTextureFromFile("Textures/leg_dif.png");

	while (Window::GetInstance().IsActive())
	{
		Time::Update();
		Input::Update();
		Camera::GetInstance().Update();
		Window::GetInstance().Update();
	}

	return 0;
}