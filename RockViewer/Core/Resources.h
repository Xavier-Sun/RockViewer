#ifndef RESOURCES_H
#define RESOURCES_H

#include <vector>
#include <memory>

#include <Graphics/Mesh.h>
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <Graphics/Material.h>

class Resources
{
public:
	std::vector<std::shared_ptr<Mesh>> meshVector;
	std::vector<std::shared_ptr<Shader>> shaderVector;
	std::vector<std::shared_ptr<Texture>> textureVector;
	std::vector<std::shared_ptr<Material>> materialVector;

	static Resources& GetInstance()
	{
		static Resources instance;
		return instance;
	}

	void ImportModelFromFile(const char* filePath);
	void ImportShaderFromFile(const char* jsonFilePath);
	void ImportTextureFromFile(const char* filePath);

private:
	Resources() = default;
	Resources(const Resources&) = delete;
	Resources& operator=(const Resources&) = delete;
};

#endif