#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	static Camera& GetInstance()
	{
		static Camera instance;
		return instance;
	}

	Camera();
	
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	void Init();
	void Update();

private:
	glm::vec3 position;

	const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	
	float yaw;
	float pitch;

	float movementSpeed;
	float rotationSpeed;
	float zoom;

	void UpdateBasisVectors()
	{
		front = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
};

#endif