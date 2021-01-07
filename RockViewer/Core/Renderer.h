#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

class Mesh;
class Shader;
class Texture;
class Material;

class Renderer
{
public:
	enum class CullMode
	{
		None,
		Front,
		Back,
		FrontAndBack
	};

	static Renderer& GetInstance()
	{
		static Renderer instance;
		return instance;
	}

	void SetWireframeMode(bool open) const;
	void SetCullMode(CullMode mode) const;
	void SetDepthTest(bool open) const;

	void NewFrame() const;
	void DrawMesh(Material& material, Mesh& mesh) const;

private:
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	Renderer();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
};

#endif