#include "Window.h"

#include <stdio.h>

#include <glm/glm.hpp>

#include <ThirdParty/imgui/imgui.h>
#include <ThirdParty/imgui/imgui_impl_glfw.h>
#include <ThirdParty/imgui/imgui_impl_opengl3.h>

#include "Resources.h"
#include "Renderer.h"
#include "Camera.h"
#include "Light.h"

void FramebufferSizeCallback(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool Window::Init()
{
	// init glfw
	if (glfwInit() == GLFW_FALSE)
	{
		printf_s("ERROR: Failed to initialize GLFW.\n");
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	glfwWindow = glfwCreateWindow(1600, 900, "ROCK ENGINE 1.1", nullptr, nullptr);
	if (!glfwWindow)
	{
		printf_s("ERROR: Failed to create window.\n");
		return false;
	}
	glfwMakeContextCurrent(glfwWindow);
	glfwSetFramebufferSizeCallback(glfwWindow, FramebufferSizeCallback);

	// init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf_s("ERROR: Failed to initialize GLAD.\n");
		return false;
	}

	// init framebuffer
	frameBuffer.Upload();

	// init imgui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigWindowsResizeFromEdges = false;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.Fonts->AddFontFromFileTTF("Resources/Fonts/SourceCodePro.ttf", 24.0f);
	if (!ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true) || !ImGui_ImplOpenGL3_Init("#version 330"))
	{
		printf_s("ERROR: Failed to initialize imgui.\n");
		return false;
	}

	return true;
}

void Window::Update()
{
	// update framebuffer
	Renderer::GetInstance().NewFrame();
	for (size_t meshCount = 0; meshCount < Resources::GetInstance().meshVector.size(); ++meshCount)
	{
		Renderer::GetInstance().DrawMesh(*Resources::GetInstance().materialVector[materialIndexVector[meshCount]], *Resources::GetInstance().meshVector[meshCount]);
	}
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// update imgui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (canShowSettingsWindow)
	{
		ShowSettingsWindow();
	}
	if (canShowSceneWindow)
	{
		ShowSceneWindow();
	}
	if (canShowResourcesWindow)
	{
		ShowResourcesWindow();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwPollEvents();
	glfwSwapBuffers(glfwWindow);
}

Window::~Window()
{
	glfwTerminate();
}

Window::Window()
{
	glfwWindow = nullptr;
	materialIndexVector = std::vector<size_t>(8);
}

void Window::ShowSceneWindow()
{
	ImGui::Begin("Scene", &canShowSceneWindow);

	ImGui::GetWindowDrawList()->AddImage
	(
		(ImTextureID)frameBuffer.GetRenderTexture(),
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2
		(
			ImGui::GetCursorScreenPos().x + ImGui::GetWindowContentRegionWidth(),
			ImGui::GetCursorScreenPos().y + ImGui::GetWindowContentRegionWidth()
			* frameBuffer.GetHeight() / frameBuffer.GetWidth()
		),
		ImVec2(0.0f, 1.0f),
		ImVec2(1.0f, 0.0f)
	);

	ImGui::End();
}

void Window::ShowSettingsWindow()
{
	ImGui::Begin("Settings", &canShowSettingsWindow);

	if (ImGui::CollapsingHeader("Material"))
	{
		for (size_t meshCount = 0; meshCount < Resources::GetInstance().meshVector.size(); ++meshCount)
		{
			auto& mesh = Resources::GetInstance().meshVector[meshCount];
			ImGui::Text(mesh->name.c_str());
			ImGui::SameLine();
			ImGui::Button(Resources::GetInstance().materialVector[materialIndexVector[meshCount]]->name.c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MATERIAL_INDEX"))
				{
					size_t payload_n = *(const size_t*)payload->Data;
					materialIndexVector[meshCount] = payload_n;
				}
				ImGui::EndDragDropTarget();
			}
		}
	}
	if (ImGui::CollapsingHeader("Camera"))
	{
		static glm::vec3 cameraPosVec3;
		static float cameraPos[3]{};
		cameraPosVec3 = Camera::GetInstance().position;
		cameraPos[0] = cameraPosVec3.x;
		cameraPos[1] = cameraPosVec3.y;
		cameraPos[2] = cameraPosVec3.z;
		if (ImGui::InputFloat3("Camera Position", cameraPos, 1))
		{
			cameraPosVec3 = glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]);
			Camera::GetInstance().position = cameraPosVec3;
		}
	}
	if (ImGui::CollapsingHeader("Light"))
	{
		static glm::vec3 lightPosVec3;
		static float lightPos[3]{};
		lightPosVec3 = Light::GetInstance().direction;
		lightPos[0] = lightPosVec3.x;
		lightPos[1] = lightPosVec3.y;
		lightPos[2] = lightPosVec3.z;
		if (ImGui::InputFloat3("Light Direction", lightPos, 1))
		{
			lightPosVec3 = glm::vec3(lightPos[0], lightPos[1], lightPos[2]);
			Light::GetInstance().direction = lightPosVec3;
		}

		static glm::vec3 lightColorVec3;
		static float lightColor[3]{};
		lightColorVec3 = Light::GetInstance().color;
		lightColor[0] = lightColorVec3.x;
		lightColor[1] = lightColorVec3.y;
		lightColor[2] = lightColorVec3.z;
		if (ImGui::ColorEdit3("Light Color", lightColor))
		{
			lightColorVec3 = glm::vec3(lightColor[0], lightColor[1], lightColor[2]);
			Light::GetInstance().color = lightColorVec3;
		}
	}
	if (ImGui::CollapsingHeader("Renderer"))
	{
		ImGui::BulletText("Depth Test");
		static bool depthTest = true;
		if (ImGui::Checkbox("Depth Test", &depthTest))
		{
			Renderer::GetInstance().SetDepthTest(depthTest);
		}

		ImGui::Separator();
		ImGui::BulletText("Polygon Mode");
		static bool wireframeMode = false;
		if (ImGui::Checkbox("Wireframe Mode", &wireframeMode))
		{
			Renderer::GetInstance().SetWireframeMode(wireframeMode);
		}

		ImGui::Separator();
		ImGui::BulletText("Cull Mode");
		static Renderer::CullMode cullMode = Renderer::CullMode::Back;
		if (ImGui::RadioButton("None", cullMode == Renderer::CullMode::None))
		{
			cullMode = Renderer::CullMode::None;
			Renderer::GetInstance().SetCullMode(cullMode);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Front", cullMode == Renderer::CullMode::Front))
		{
			cullMode = Renderer::CullMode::Front;
			Renderer::GetInstance().SetCullMode(cullMode);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Back", cullMode == Renderer::CullMode::Back))
		{
			cullMode = Renderer::CullMode::Back;
			Renderer::GetInstance().SetCullMode(cullMode);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("FrontAndBack", cullMode == Renderer::CullMode::FrontAndBack))
		{
			cullMode = Renderer::CullMode::FrontAndBack;
			Renderer::GetInstance().SetCullMode(cullMode);
		}
	}

	ImGui::End();
}

void Window::ShowResourcesWindow()
{
	ImGui::Begin("Resources", &canShowResourcesWindow);

	if (ImGui::CollapsingHeader("Material"))
	{
		for (size_t materialCount = 0; materialCount < Resources::GetInstance().materialVector.size(); ++materialCount)
		{
			auto& material = Resources::GetInstance().materialVector[materialCount];
			ImGui::PushID(materialCount);
			ImGui::Button(material->name.c_str(), ImVec2(60.0f, 30.0f));
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("MATERIAL_INDEX", &materialCount, sizeof(size_t));
				ImGui::Text(material->name.c_str());
				ImGui::EndDragDropSource();
			}
			ImGui::PopID();
		}
	}
	if (ImGui::CollapsingHeader("Texture"))
	{
		for (size_t textureCount = 0; textureCount < Resources::GetInstance().textureVector.size(); ++textureCount)
		{
			ImGui::PushID(textureCount);
			ImGui::ImageButton((ImTextureID)Resources::GetInstance().textureVector[textureCount]->GetID(), ImVec2(60, 60));
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("TEXTURE_INDEX", &textureCount, sizeof(size_t));
				ImGui::Text(Resources::GetInstance().textureVector[textureCount]->name.c_str());
				ImGui::EndDragDropSource();
			}
			ImGui::SameLine();
			ImGui::Text(Resources::GetInstance().textureVector[textureCount]->name.c_str());
			ImGui::PopID();
		}
	}

	ImGui::End();
}
