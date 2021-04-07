#include "camera.h"

Camera::Camera()
	: m_movement_speed(2.5f), 
	m_mouse_sensitivity(0.1f),
	m_fov(45.0f),
	m_position(Vector3(0.0f, 0.0f, 3.0f)),
	m_world_up(Vector3(0.0f, 1.0f, 0.0f)),
	m_yaw(-90.f),
	m_pitch(0.0f),
	m_front(Vector3(0.0f, 0.0f, -1.0f))
{
	updateCameraVectors();
}

void Camera::ProcessKeyboard(CameraOperator direction, float deltaTime)
{
	float velocity = m_movement_speed * deltaTime;
	switch (direction)
	{
	case FORWARD:
		m_position += m_front * velocity;
		break;
	case BACKWARD:
		m_position -= m_front * velocity;
		break;
	case LEFT:
		m_position -= m_right * velocity;
		break;
	case RIGHT:
		m_position += m_right * velocity;
		break;
	}
	m_position.y = 0.0f;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= m_mouse_sensitivity;
	yoffset *= m_mouse_sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
		else if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}
	}
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	m_fov -= (float)yoffset;
	if (m_fov < 1.0f)
	{
		m_fov = 1.0f;
	}
	else if (m_fov > 45.0f)
	{
		m_fov == 45.0f;
	}
}

void Camera::updateCameraVectors()
{
	Vector3 front;
	front.x = cos(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_world_up));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

Matrix4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}