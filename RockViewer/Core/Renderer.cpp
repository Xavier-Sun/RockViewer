#include "Renderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <Graphics/Material.h>
#include <Graphics/Mesh.h>

#include "Camera.h"
#include "Window.h"
#include "Light.h"

Renderer::Renderer()
{
	SetCullMode(CullMode::Back);
	SetDepthTest(true);
}

void Renderer::SetWireframeMode(bool open) const
{
	if (open)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Renderer::SetCullMode(CullMode mode) const
{
	switch (mode)
	{
	case Renderer::CullMode::None:
		glDisable(GL_CULL_FACE);
		break;
	case Renderer::CullMode::Front:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	case Renderer::CullMode::Back:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	case Renderer::CullMode::FrontAndBack:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
		break;
	default:
		break;
	}
}

void Renderer::SetDepthTest(bool open) const
{
	if (open)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void Renderer::NewFrame() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, Window::GetInstance().frameBuffer.GetFBO());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawMesh(Material& material, Mesh& mesh) const
{
	material.shader->Use();
	material.shader->SetMat4("Projection", Camera::GetInstance().GetProjectionMatrix());
	material.shader->SetMat4("View", Camera::GetInstance().GetViewMatrix());
	material.shader->SetMat4("Model", modelMatrix);
	material.shader->SetVec3("LightDirection", Light::GetInstance().direction);
	material.shader->SetVec3("LightColor", Light::GetInstance().color);
	material.shader->SetVec3("CameraPosition", Camera::GetInstance().position);
	material.SetShaderProperties();

	glBindVertexArray(mesh.GetVAO());
	glDrawElements(GL_TRIANGLES, mesh.GetIndexNum(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
