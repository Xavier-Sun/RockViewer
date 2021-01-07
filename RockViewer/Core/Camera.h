#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	glm::vec3 position = glm::vec3(0.0f, 10.0f, 20.0f);

	float movementSpeed = 0.0f;
	float rotationSpeed = 0.0f;
	float zoom = 0.0f;

	static Camera& GetInstance()
	{
		static Camera instance;
		return instance;
	}

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	void Init();
	void Update();

private:
	const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// basis vectors
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	
	float yaw = -90.0f;
	float pitch = 0.0f;

	Camera();
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	void UpdateBasisVectors();
};

#endif