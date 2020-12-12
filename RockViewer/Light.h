#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light
{
public:
	static Light& GetInstance()
	{
		static Light instance;
		return instance;
	}

	glm::vec3 GetDirection() const { return direction; }
	void SetDirection(const glm::vec3& dir) { direction = dir; }

	glm::vec3 GetColor() const { return color; }
	void SetColor(const glm::vec3& col) { color = col; }

private:
	glm::vec3 direction;
	glm::vec3 color;

	Light();
	Light(const Light&) = delete;
};

#endif