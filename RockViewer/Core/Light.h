#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light
{
public:
	glm::vec3 direction = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	static Light& GetInstance()
	{
		static Light instance;
		return instance;
	}

private:
	Light();
	Light(const Light&) = delete;
	Light& operator=(const Light&) = delete;
};

#endif