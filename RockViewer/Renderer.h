#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

class Renderer
{
public:
	static Renderer& GetInstance()
	{
		static Renderer instance;
		return instance;
	}

	void Render(Shader& shader);

private:
	glm::mat4 modelMatrix;

	Renderer();
	Renderer(Renderer&) = delete;
};

#endif