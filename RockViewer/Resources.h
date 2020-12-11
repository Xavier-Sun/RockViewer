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

	int GetMeshCount() { return meshVector.size(); }
	int GetShaderCount() { return shaderVector.size(); }

	int GetVertexCount() { return vertexCount; }
	int GetIndexCount() { return indexCount; }

	bool HasNormal() { return hasNormal; }
	bool HasUV0() { return hasUV0; }
	bool HasUV1() { return hasUV1; }

	bool hasAnyMesh() { return !meshVector.empty(); }
	bool hasAnyShader() { return !shaderVector.empty(); }

	void LoadModelFrom(const char* filePath);
	void LoadShaderFrom(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);

private:
	int vertexCount;
	int indexCount;
	bool hasNormal;
	bool hasUV0;
	bool hasUV1;

	Resources();
	Resources(const Resources&) = delete;

	void ResetMeshMessage();
};

#endif