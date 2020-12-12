#include "Resources.h"

#include <stdio.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <sstream>

void Resources::LoadModelFrom(const char* filePath)
{
	ResetMeshMessage();

	Assimp::Importer importer;
	const aiScene* aiscene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!aiscene || aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiscene->mRootNode)
	{
		printf_s("模型加载失败。错误信息：\n%s\n", importer.GetErrorString());
		return;
	}

	for (unsigned int i = 0; i < aiscene->mNumMeshes; ++i)
	{
		auto aimesh = aiscene->mMeshes[i];
		Mesh mesh;
		for (unsigned int j = 0; j < aimesh->mNumVertices; ++j)
		{
			Vertex vertex{};
			vertex.position = glm::vec3(aimesh->mVertices[j].x, aimesh->mVertices[j].y, aimesh->mVertices[j].z);
			if (aimesh->HasNormals())
			{
				hasNormal = true;
				vertex.normal = glm::vec3(aimesh->mNormals[j].x, aimesh->mNormals[j].y, aimesh->mNormals[j].z);
			}
			if (aimesh->mTextureCoords[0])
			{
				hasUV0 = true;
				vertex.texcoord = glm::vec2(aimesh->mTextureCoords[0][j].x, aimesh->mTextureCoords[0][j].y);
				vertex.tangent = glm::vec3(aimesh->mTangents[j].x, aimesh->mTangents[j].y, aimesh->mTangents[j].z);
				vertex.bitangent = glm::vec3(aimesh->mBitangents[j].x, aimesh->mBitangents[j].y, aimesh->mBitangents[j].z);
			}
			else
			{
				vertex.texcoord = glm::vec2(0.0f, 0.0f);
			}
			mesh.vertexVector.push_back(vertex);
		}
		for (unsigned int j = 0; j < aimesh->mNumFaces; ++j)
		{
			aiFace aiface = aimesh->mFaces[j];
			for (unsigned int k = 0; k < aiface.mNumIndices; ++k)
			{
				mesh.indexVector.push_back(aiface.mIndices[k]);
			}
		}
		vertexCount += mesh.vertexVector.size();
		indexCount += mesh.indexVector.size();
		mesh.UploadMeshData();
		meshVector.push_back(mesh);

		printf_s("成功导入网格。模型路径：%s，顶点数量：%d，索引数量：%d\n",
			filePath,
			static_cast<int>(mesh.vertexVector.size()),
			static_cast<int>(mesh.indexVector.size()));
	}
}

void Resources::LoadShaderFrom(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
	std::string vertString;
	std::string fragString;
	std::ifstream vertFileStream;
	std::ifstream fragFileStream;
	std::stringstream vertStringStream;
	std::stringstream fragStringStream;
	vertFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertFileStream.open(vertexShaderFilePath);
		fragFileStream.open(fragmentShaderFilePath);
		vertStringStream << vertFileStream.rdbuf();
		fragStringStream << fragFileStream.rdbuf();
		vertFileStream.close();
		fragFileStream.close();
		vertString = vertStringStream.str();
		fragString = fragStringStream.str();
	}
	catch (std::ifstream::failure&)
	{
		printf_s("着色器文件读取失败。\n");
	}

	auto shader = Shader();
	shader.Load(vertString.c_str(), fragString.c_str());
	shaderVector.push_back(shader);

	printf_s("成功加载着色器。编号：%d。\n", static_cast<int>(shaderVector.size()) - 1);
}

Resources::Resources()
{
	ResetMeshMessage();
}

void Resources::ResetMeshMessage()
{
	vertexCount = 0;
	indexCount = 0;
	hasNormal = false;
	hasUV0 = false;
	hasUV1 = false;
	meshVector.clear();
}