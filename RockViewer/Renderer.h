#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

class Mesh;
class Shader;
class Texture;

class Renderer
{
public:
	static Renderer& GetInstance()
	{
		static Renderer instance;
		return instance;
	}

	void Render(Shader& shader) const;
	void RenderInit(Shader& shader) const;
	void Render(Mesh& mesh, Texture& texture) const;
	void RenderEnd() const;

private:
	glm::mat4 modelMatrix;

	Renderer();
	Renderer(const Renderer&) = delete;
};

#endif