#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <string>
#include <memory>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Shader.h"

class Material
{
public:
	std::string name;

	std::shared_ptr<Shader> shader;

	Material() = default;
	~Material() = default;

	void SetTexture(const std::string& name, const std::shared_ptr<Texture>& value);
	void SetVec3(const std::string& name, const glm::vec3& value);
	void SetMat4(const std::string& name, const glm::mat4& value);

	void SetShaderProperties() const;

private:
	std::map<std::string, std::shared_ptr<Texture>> textureMap;
	std::map<std::string, glm::vec3> vec3Map;
	std::map<std::string, glm::mat4> mat4Map;
};

#endif