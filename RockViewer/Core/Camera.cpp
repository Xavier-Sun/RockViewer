#include "Camera.h"

#include "Input.h"
#include "Time.h"

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
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
		zoom = zoom < 1.0f ? 1.0f : zoom > 45.0f ? 45.0f : zoom;

		UpdateBasisVectors();
	}
}

Camera::Camera()
{
	UpdateBasisVectors();
}

void Camera::UpdateBasisVectors()
{
	front = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
