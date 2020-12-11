#include "Window.h"

#include <stdio.h>

#include "Resources.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
	if (!ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true) || !ImGui_ImplOpenGL3_Init("#version 330"))
	{
		printf_s("´íÎó£ºImGui³õÊ¼»¯Ê§°Ü¡£\n");
		return false;
	}

	return true;
}

void Window::Update()
{
	static int currentShader = 0;
	Renderer::GetInstance().Render(Resources::GetInstance().shaderVector[currentShader]);

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

	static char modelFilePath[50];
	ImGui::InputText("Model File Path", modelFilePath, 50ull);

	if (ImGui::Button("Load Model"))
	{
		Resources::GetInstance().LoadModelFrom(modelFilePath);
	}

	if (ImGui::CollapsingHeader("Mesh Data"))
	{
		ImGui::BulletText("Vertex Number: %d", Resources::GetInstance().GetVertexCount());
		ImGui::BulletText("Index Number: %d", Resources::GetInstance().GetIndexCount());
		static std::string yesOrNo = Resources::GetInstance().HasNormal() ? "Yes" : "No";
		ImGui::BulletText("Has Normal: %s", yesOrNo.c_str());
		yesOrNo = Resources::GetInstance().HasUV0() ? "Yes" : "No";
		ImGui::BulletText("Has UV 0: %s", yesOrNo.c_str());
		yesOrNo = Resources::GetInstance().HasUV1() ? "Yes" : "No";
		ImGui::BulletText("Has UV 1: %s", yesOrNo.c_str());
	}

	ImGui::End();
}