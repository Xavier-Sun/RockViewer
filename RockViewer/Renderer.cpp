#include "Renderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Resources.h"
#include "Window.h"

Renderer::Renderer()
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
}

void Renderer::Render(Shader& shader) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, Window::GetInstance().frameBuffer.FBO);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Use();
	shader.SetMat4("Projection", Camera::GetInstance().GetProjectionMatrix());
	shader.SetMat4("View", Camera::GetInstance().GetViewMatrix());
	shader.SetMat4("Model", modelMatrix);

	for (Mesh& mesh : Resources::GetInstance().meshVector)
	{
		glBindVertexArray(mesh.GetVAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.GetEBO());
		glDrawElements(GL_TRIANGLES, mesh.indexVector.size(), GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}