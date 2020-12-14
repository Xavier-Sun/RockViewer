#include "Resources.h"

#include <stdio.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <sstream>

#include "stb_image.h"

Resources::~Resources()
{
	ResetMeshMessage();
	ResetShaderMessage();
	ResetTextureMessage();
}

void Resources::LoadModelFromFile(const char* filePath)
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
		meshVector.emplace_back();
		Mesh& mesh = meshVector[meshVector.size() - 1];
		mesh.Create();
		aiMesh* aimesh = aiscene->mMeshes[i];
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
		mesh.Upload();

		printf_s("成功导入网格。模型路径：%s，顶点数量：%d，索引数量：%d\n",
			filePath,
			static_cast<int>(mesh.vertexVector.size()),
			static_cast<int>(mesh.indexVector.size()));
	}
}

void Resources::LoadShaderFromFile(const std::string& shaderName, const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
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

	shaderVector.emplace_back();
	Shader& shader = shaderVector[shaderVector.size() - 1];
	shader.Create();
	shader.Upload(vertString.c_str(), fragString.c_str());
	shader.SetName(shaderName);

	printf_s("成功导入着色器。名称：%s。\n", shaderName.c_str());
}

void Resources::LoadTextureFromFile(const char* filePath)
{
	stbi_set_flip_vertically_on_load(true);
	int width;
	int height;
	int channelNum;
	unsigned char* data = stbi_load(filePath, &width, &height, &channelNum, 0);
	if (data)
	{
		textureVector.emplace_back();
		Texture& texture = textureVector[textureVector.size() - 1];
		texture.Create();
		texture.Upload(width, height, data);
		texture.SetName(filePath);

		printf_s("成功导入纹理。名称：%s，宽度：%d，高度：%d\n", filePath, width, height);
	}
	else
	{
		printf_s("纹理导入失败。");
	}
	stbi_image_free(data);
}

Resources::Resources()
{
	vertexCount = 0;
	indexCount = 0;
	hasNormal = false;
	hasUV0 = false;
}

void Resources::ResetMeshMessage()
{
	vertexCount = 0;
	indexCount = 0;
	hasNormal = false;
	hasUV0 = false;
	for (auto& mesh : meshVector)
	{
		mesh.Destroy();
	}
	meshVector.clear();
}

void Resources::ResetShaderMessage()
{
	for (auto& shader : shaderVector)
	{
		shader.Destroy();
	}
	shaderVector.clear();
}

void Resources::ResetTextureMessage()
{
	for (auto& texture : textureVector)
	{
		texture.Destroy();
	}
	textureVector.clear();
}