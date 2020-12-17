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

bool Window::Init()
{
	if (InitGLFW() && InitGLAD() && InitFrameBuffer() && InitGUI())
	{
		return true;
	}
	return false;
}

void Window::Update()
{
	UpdateFrameBuffer();
	UpdateGUI();

	glfwPollEvents();
	glfwSwapBuffers(glfwWindow);
}

Window::Window()
{
	glfwWindow = nullptr;
	currentShaderIndex = 0ull;
	currentTextureIndex = std::vector<size_t>(20);
}

bool Window::InitGLFW()
{
	if (glfwInit() == GLFW_FALSE)
	{
		printf_s("GLFW初始化失败。");
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindow = glfwCreateWindow(1600, 900, "ROCK ENGINE 1.0", nullptr, nullptr);
	if (!glfwWindow)
	{
		printf_s("错误：GLFW窗口创建失败。");
		return false;
	}
	glfwMakeContextCurrent(glfwWindow);
	glfwSetFramebufferSizeCallback(glfwWindow, FramebufferSizeCallback);

	return true;
}

bool Window::InitGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf_s("错误：GLAD初始化失败。");
		return false;
	}

	return true;
}

bool Window::InitFrameBuffer()
{
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
		printf_s("错误：创建的帧缓冲不完整。\n");
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

bool Window::InitGUI()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsResizeFromEdges = false;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.Fonts->AddFontFromFileTTF("Fonts/SourceCodePro.ttf", 24.0f);
	if (!ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true) || !ImGui_ImplOpenGL3_Init("#version 330"))
	{
		printf_s("错误：ImGui初始化失败。\n");
		return false;
	}

	return true;
}

void Window::UpdateFrameBuffer()
{
	Renderer::GetInstance().RenderInit(Resources::GetInstance().shaderVector[currentShaderIndex]);
	for (size_t meshCount = 0; meshCount < Resources::GetInstance().meshVector.size(); ++meshCount)
	{
		Renderer::GetInstance().Render
		(
			Resources::GetInstance().meshVector[meshCount],
			Resources::GetInstance().textureVector[currentTextureIndex[meshCount]]
		);
	}
	Renderer::GetInstance().RenderEnd();
}

void Window::UpdateGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (gui.canShowMessageWindow)
	{
		ShowMessageWindow();
	}
	if (gui.canShowSceneWindow)
	{
		ShowSceneWindow();
	}
	if (gui.canShowResourcesWindow)
	{
		ShowResourcesWindow();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

	if (ImGui::CollapsingHeader("Texture Settings"))
	{
		for (size_t meshCount = 0; meshCount < Resources::GetInstance().GetMeshCount(); meshCount++)
		{
			static std::string title;
			title = "Mesh" + std::to_string(meshCount);
			ImGui::Text(title.c_str());
			ImGui::SameLine();
			ImGui::Button(Resources::GetInstance().textureVector[currentTextureIndex[meshCount]].GetName().c_str(), ImVec2(300, 30));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_INDEX"))
				{
					size_t payload_n = *(const size_t*)payload->Data;
					currentTextureIndex[meshCount] = payload_n;
				}
				ImGui::EndDragDropTarget();
			}
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
		if (ImGui::InputFloat3("Light Direction", lightPos, 1))
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
		if (ImGui::DragFloat3("Light Color", lightColor, 0.01f, 0.0f, 1.0f, "%.2f"))
		{
			lightColorVec3 = glm::vec3(lightColor[0], lightColor[1], lightColor[2]);
			Light::GetInstance().SetColor(lightColorVec3);
		}
	}
	if (ImGui::CollapsingHeader("Render Settings"))
	{
		static bool borderOnly = false;
		if (ImGui::Checkbox("Border Only", &borderOnly))
		{
			Renderer::GetInstance().SetBorderOnly(borderOnly);
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

void Window::ShowResourcesWindow()
{
	ImGui::Begin("Resources", &gui.canShowResourcesWindow);

	if (ImGui::CollapsingHeader("Models"))
	{
		static char modelFilePath[50];
		ImGui::InputText("Model File Path", modelFilePath, sizeof(modelFilePath));
		if (ImGui::Button("Load Model", ImVec2(300.0f, 30.0f)))
		{
			Resources::GetInstance().LoadModelFromFile(modelFilePath);
		}
	}
	if (ImGui::CollapsingHeader("Textures"))
	{
		static char textureFilePath[50];
		ImGui::InputText("Texture File Path", textureFilePath, sizeof(textureFilePath));
		if (ImGui::Button("Load Texture", ImVec2(300.0f, 30.0f)))
		{
			Resources::GetInstance().LoadTextureFromFile(textureFilePath);
		}
		for (size_t textureCount = 0; textureCount < Resources::GetInstance().textureVector.size(); ++textureCount)
		{
			ImGui::PushID(textureCount);
			ImGui::Button(Resources::GetInstance().textureVector[textureCount].GetName().c_str(), ImVec2(300, 60));
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("TEXTURE_INDEX", &textureCount, sizeof(size_t));
				ImGui::Text(Resources::GetInstance().textureVector[textureCount].GetName().c_str());
				ImGui::EndDragDropSource();
			}
			ImGui::PopID();
		}
	}

	ImGui::End();
}