#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0.0f, 10.0f, 20.0f);
	yaw = -90.0f;
	pitch = 0.0f;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	right = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	movementSpeed = 0.0f;
	rotationSpeed = 0.0f;
	zoom = 0.0f;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(zoom), 16.0f / 9.0f, 0.1f, 100.0f);
}

void Camera::Init()
{
	UpdateBasisVectors();
	movementSpeed = 2.5f;
	rotationSpeed = 0.1f;
	zoom = 45.0f;
}

void Camera::Update()
{
	if (Input::GetMouseButton(1))
	{
		float offset = movementSpeed * static_cast<float>(Time::GetDeltaTime());
		if (Input::GetKey(KeyCode::W))
		{
			position += front * offset;
		}
		if (Input::GetKey(KeyCode::S))
		{
			position -= front * offset;
		}
		if (Input::GetKey(KeyCode::A))
		{
			position -= right * offset;
		}
		if (Input::GetKey(KeyCode::D))
		{
			position += right * offset;
		}
		yaw += Input::GetAxisRaw(AxisType::MouseX) * rotationSpeed;
		pitch -= Input::GetAxisRaw(AxisType::MouseY) * rotationSpeed;

		pitch = pitch > 89.0f ? 89.0f : pitch < -89.0f ? -89.0f : pitch;
		zoom -= Input::GetAxisRaw(AxisType::ScrollY);
		zoom = zoom < 1.0f ? 1.0f : zoom > 45.0f ? 45.0f : zoom;

		UpdateBasisVectors();
	}
}