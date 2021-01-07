#include "Resources.h"

#include <stdio.h>
#include <fstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <ThirdParty/stb_image/stb_image.h>
#include <json/json.hpp>

#include <Utilities/File.h>

void Resources::ImportModelFromFile(const char* filePath)
{
	Assimp::Importer importer;
	const aiScene* aiscene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!aiscene || aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiscene->mRootNode)
	{
		printf_s("ERROR: Failed to load model. Message:\n%s\n", importer.GetErrorString());
		return;
	}

	for (unsigned int i = 0; i < aiscene->mNumMeshes; ++i)
	{
		aiMesh* aimesh = aiscene->mMeshes[i];

		std::vector<Vertex> vertexVector;
		std::vector<unsigned int> indexVector;

		// import vertices
		for (unsigned int j = 0; j < aimesh->mNumVertices; ++j)
		{
			Vertex& vertex = vertexVector.emplace_back();

			// import position
			vertex.position = glm::vec3(aimesh->mVertices[j].x, aimesh->mVertices[j].y, aimesh->mVertices[j].z);
			
			// import normal
			if (aimesh->HasNormals())
			{
				vertex.normal = glm::vec3(aimesh->mNormals[j].x, aimesh->mNormals[j].y, aimesh->mNormals[j].z);
			}

			// import uv
			if (aimesh->mTextureCoords[0])
			{
				vertex.texcoord = glm::vec2(aimesh->mTextureCoords[0][j].x, aimesh->mTextureCoords[0][j].y);
			}
			else
			{
				vertex.texcoord = glm::vec2(0.0f, 0.0f);
			}

			// import tangent
			if (aimesh->mTangents)
			{
				vertex.tangent = glm::vec3(aimesh->mTangents[j].x, aimesh->mTangents[j].y, aimesh->mTangents[j].z);
			}

			// import bitangent
			if (aimesh->mBitangents)
			{
				vertex.bitangent = glm::vec3(aimesh->mBitangents[j].x, aimesh->mBitangents[j].y, aimesh->mBitangents[j].z);
			}
		}

		// import indices
		for (unsigned int j = 0; j < aimesh->mNumFaces; ++j)
		{
			aiFace aiface = aimesh->mFaces[j];
			for (unsigned int k = 0; k < aiface.mNumIndices; ++k)
			{
				indexVector.push_back(aiface.mIndices[k]);
			}
		}

		std::shared_ptr<Mesh>& mesh = meshVector.emplace_back();
		mesh = std::make_shared<Mesh>();
		mesh->Upload(vertexVector, indexVector);
		mesh->name = aimesh->mName.C_Str();

		printf_s("New mesh imported: Name£º%s£¬Vertex Number£º%d£¬Index Number£º%d\n",
			mesh->name.c_str(), static_cast<int>(mesh->GetVertexNum()), static_cast<int>(mesh->GetIndexNum()));
	}
}

void Resources::ImportShaderFromFile(const char* jsonFilePath)
{
	nlohmann::json jsonContent;

	std::ifstream file(jsonFilePath);
	file >> jsonContent;

	std::string vertFilePath = jsonContent["vert"].get<std::string>();
	std::string fragFilePath = jsonContent["frag"].get<std::string>();

	std::shared_ptr<std::string> vert = File::ReadFileToString(vertFilePath.c_str());
	std::shared_ptr<std::string> frag = File::ReadFileToString(fragFilePath.c_str());

	std::shared_ptr<Shader>& shader = shaderVector.emplace_back();
	shader = std::make_shared<Shader>();
	shader->Upload(vert->c_str(), frag->c_str());
	shader->name = jsonContent["name"].get<std::string>();
	shader->properties = jsonContent["properties"].get<decltype(shader->properties)>();

	printf_s("New shader imported: Name£º%s\n", shader->name.c_str());
}

void Resources::ImportTextureFromFile(const char* filePath)
{
	stbi_set_flip_vertically_on_load(true);

	int width = 0;
	int height = 0;
	int channelNum = 0;

	unsigned char* textureData = stbi_load(filePath, &width, &height, &channelNum, 0);

	if (textureData)
	{
		std::shared_ptr<Texture>& texture = textureVector.emplace_back();
		texture = std::make_shared<Texture>();
		texture->Upload(width, height, textureData);

		printf_s("New texture imported: Name£º%s£¬Size£º%d x %d\n", texture->name.c_str(), width, height);
	}
	else
	{
		printf_s("ERROR: Failed to import texture. File Path: %s\n", filePath);
	}

	stbi_image_free(textureData);
}
