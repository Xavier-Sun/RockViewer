#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

class Shader;

class Renderer
{
public:
	static Renderer& GetInstance()
	{
		static Renderer instance;
		return instance;
	}

	void Render(Shader& shader) const;

private:
	glm::mat4 modelMatrix;

	Renderer();
	Renderer(const Renderer&) = delete;
};

#endif