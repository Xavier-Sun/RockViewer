#include "Material.h"

#include <glad/glad.h>

void Material::SetTexture(const std::string& name, const std::shared_ptr<Texture>& value)
{
	textureMap[name] = value;
}

void Material::SetVec3(const std::string& name, const glm::vec3& value)
{
	vec3Map[name] = value;
}

void Material::SetMat4(const std::string& name, const glm::mat4& value)
{
	mat4Map[name] = value;
}

void Material::SetShaderProperties() const
{
	shader->Use();

	int textureCount = 0;
	for (auto& item : textureMap)
	{		
		glActiveTexture(GL_TEXTURE0 + textureCount);
		glBindTexture(GL_TEXTURE_2D, item.second->GetID());
		shader->SetInt(item.first, textureCount++);
	}
	glActiveTexture(GL_TEXTURE0);

	for (auto& item : vec3Map)
	{
		shader->SetVec3(item.first, item.second);
	}

	for (auto& item : mat4Map)
	{
		shader->SetMat4(item.first, item.second);
	}
}
