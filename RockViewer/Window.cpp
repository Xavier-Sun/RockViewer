#include "Window.h"

#include <stdio.h>
#include <glm/glm.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Resources.h"
#include "Renderer.h"
#include "Camera.h"
#include "Light.h"

Window::~Window()
{
	glDeleteFramebuffers(1, &frameBuffer.FBO);
	glDeleteTextures(1, &frameBuffer.renderTexture);
	glDeleteRenderbuffers(1, &frameBuffer.RBO);
	glfwTerminate();
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool Window::Init(int width, int height, const char* title)
{
	if (glfwInit() == GLFW_FALSE)
	{
		printf_s("GLFW³õÊ¼»¯Ê§°Ü¡£");
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!glfwWindow)
	{
		printf_s("´íÎó£ºGLFW´°¿Ú´´½¨Ê§°Ü¡£");
		return false;
	}
	glfwMakeContextCurrent(glfwWindow);
	glfwSetFramebufferSizeCallback(glfwWindow, FramebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf_s("´íÎó£ºGLAD³õÊ¼»¯Ê§°Ü¡£");
		return false;
	}

	glGenFramebuffers(1, &frameBuffer.FBO);
	glGenTextures(1, &frameBuffer.renderTexture);
	glGenRenderbuffers(1, &frameBuffer.RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.FBO);
	glBindTexture(GL_TEXTURE_2D, frameBuffer.renderTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer.RBO);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameBuffer.width, frameBuffer.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameBuffer.width, frameBuffer.height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBuffer.renderTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBuffer.RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("´íÎó£º´´½¨µÄÖ¡»º³å²»ÍêÕû¡£\n");
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Fonts/SourceCodePro.ttf", 24.0f);
	if (!ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true) || !ImGui_ImplOpenGL3_Init("#version 330"))
	{
		printf_s("´íÎó£ºImGui³õÊ¼»¯Ê§°Ü¡£\n");
		return false;
	}

	return true;
}

void Window::Update()
{
	Renderer::GetInstance().Render(Resources::GetInstance().shaderVector[currentShaderIndex]);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (gui.canShowSceneWindow)
	{
		ShowSceneWindow();
	}
	if (gui.canShowMessageWindow)
	{
		ShowMessageWindow();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwPollEvents();
	glfwSwapBuffers(glfwWindow);
}

Window::Window()
{
	glfwWindow = nullptr;
	currentShaderIndex = 0ull;
}

void Window::ShowSceneWindow()
{
	ImGui::Begin("Scene", &gui.canShowSceneWindow);

	ImGui::GetWindowDrawList()->AddImage
	(
		(ImTextureID)frameBuffer.renderTexture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2
		(
			ImGui::GetCursorScreenPos().x + ImGui::GetWindowContentRegionWidth(),
			ImGui::GetCursorScreenPos().y + ImGui::GetWindowContentRegionWidth() * 3 / 4
		),
		ImVec2(0.0f, 1.0f),
		ImVec2(1.0f, 0.0f)
	);

	ImGui::End();
}

void Window::ShowMessageWindow()
{
	ImGui::Begin("Model Message", &gui.canShowMessageWindow);

	if (ImGui::CollapsingHeader("Load Model"))
	{
		static char modelFilePath[50];

		ImGui::InputText("Model File Path", modelFilePath, sizeof(modelFilePath));
		if (ImGui::Button("Load Model"))
		{
			Resources::GetInstance().LoadModelFromFile(modelFilePath);
		}
	}
	if (ImGui::CollapsingHeader("Camera Settings"))
	{
		static glm::vec3 cameraPosVec3;
		static float cameraPos[3]{};

		cameraPosVec3 = Camera::GetInstance().GetPosition();
		cameraPos[0] = cameraPosVec3.x;
		cameraPos[1] = cameraPosVec3.y;
		cameraPos[2] = cameraPosVec3.z;
		if (ImGui::InputFloat3("Camera Position", cameraPos, 1))
		{
			cameraPosVec3 = glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]);
			Camera::GetInstance().SetPosition(cameraPosVec3);
		}
	}
	if (ImGui::CollapsingHeader("Light Settings"))
	{
		static glm::vec3 lightPosVec3;
		static float lightPos[3]{};
		
		lightPosVec3 = Light::GetInstance().GetDirection();
		lightPos[0] = lightPosVec3.x;
		lightPos[1] = lightPosVec3.y;
		lightPos[2] = lightPosVec3.z;
		if (ImGui::InputFloat3("Light Position", lightPos, 1))
		{
			lightPosVec3 = glm::vec3(lightPos[0], lightPos[1], lightPos[2]);
			Light::GetInstance().SetDirection(lightPosVec3);
		}

		static glm::vec3 lightColorVec3;
		static float lightColor[3]{};

		lightColorVec3 = Light::GetInstance().GetColor();
		lightColor[0] = lightColorVec3.x;
		lightColor[1] = lightColorVec3.y;
		lightColor[2] = lightColorVec3.z;
		if (ImGui::InputFloat3("Light Color", lightColor, 1))
		{
			lightColorVec3 = glm::vec3(lightColor[0], lightColor[1], lightColor[2]);
			Light::GetInstance().SetColor(lightColorVec3);
		}
	}
	if (ImGui::CollapsingHeader("Shader Settings"))
	{
		if (ImGui::BeginCombo("Shader", Resources::GetInstance().shaderVector[currentShaderIndex].GetName().c_str()))
		{
			for (size_t i = 0; i < Resources::GetInstance().shaderVector.size(); ++i)
			{
				if (ImGui::Selectable(Resources::GetInstance().shaderVector[i].GetName().c_str()))
				{
					currentShaderIndex = i;
				}
			}
			ImGui::EndCombo();
		}
	}
	if (ImGui::CollapsingHeader("Mesh Data"))
	{
		static std::string yesOrNo;

		ImGui::BulletText("Mesh Number: %d", Resources::GetInstance().GetMeshCount());
		ImGui::BulletText("Vertex Number: %d", Resources::GetInstance().GetVertexCount());
		ImGui::BulletText("Index Number: %d", Resources::GetInstance().GetIndexCount());
		yesOrNo = Resources::GetInstance().HasNormal() ? "Yes" : "No";
		ImGui::BulletText("Has Normal: %s", yesOrNo.c_str());
		yesOrNo = Resources::GetInstance().HasUV0() ? "Yes" : "No";
		ImGui::BulletText("Has UV 0: %s", yesOrNo.c_str());
	}

	ImGui::End();
}