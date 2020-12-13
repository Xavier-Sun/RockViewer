#ifndef RESOURCES_H
#define RESOURCES_H

#include <vector>

#include "Mesh.h"
#include "Shader.h"

class Resources
{
public:
	std::vector<Mesh> meshVector;
	std::vector<Shader> shaderVector;

	static Resources& GetInstance()
	{
		static Resources instance;
		return instance;
	}

	int GetMeshCount() const { return meshVector.size(); }
	int GetShaderCount() const { return shaderVector.size(); }

	int GetVertexCount() const { return vertexCount; }
	int GetIndexCount() const { return indexCount; }

	bool HasNormal() const { return hasNormal; }
	bool HasUV0() const { return hasUV0; }

	bool hasAnyMesh() const { return !meshVector.empty(); }
	bool hasAnyShader() const { return !shaderVector.empty(); }

	void LoadModelFromFile(const char* filePath);
	void LoadShaderFromFile(const std::string& shaderName, const char* vertexShaderFilePath, const char* fragmentShaderFilePath);

private:
	int vertexCount;
	int indexCount;
	bool hasNormal;
	bool hasUV0;

	Resources();
	Resources(const Resources&) = delete;

	void ResetMeshMessage();
	void ResetShaderMessage();
};

#endif