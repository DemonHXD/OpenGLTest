#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../common/math.h"

enum CameraOperator
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
class Camera
{
public:

	Camera();
	~Camera() = default;

	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;

	Camera& operator=(const Camera&) = delete;
	Camera& operator=(Camera&&) = delete;

	Matrix4 GetViewMatrix() const;
	void ProcessKeyboard(CameraOperator direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
	void ProcessMouseScroll(float yoffset);

	void setPosition(Vector3 position) { m_position = position; }
	Vector3 getPosition() const { return m_position; }

	void setFront(Vector3 front) { m_front = front; }
	Vector3 getFront() const { return m_front; }

	void setUp(Vector3 up) { m_up = up; }
	Vector3 getUp() const { return m_up; }

	void setRight(Vector3 right) { m_right = right; }
	Vector3 getRight() const { return m_right; }

	void setWorldUp(Vector3 world_up) { m_world_up = world_up; }
	Vector3 getWorldUp() const { return m_world_up; }

	void setYaw(float yaw) { m_yaw = yaw; }
	float getYaw() const { return m_yaw; }

	void setPitch(float pitch) { m_pitch = pitch; }
	float getPitch() const { return m_pitch; }

	void setSpeed(float movement_speed) { m_movement_speed = movement_speed; }
	float getSpeed() const { return m_movement_speed; }

	void setSensitivity(float mouse_sensitivity) { m_mouse_sensitivity = mouse_sensitivity; }
	float getSensitivity() const { return m_mouse_sensitivity; }

	void setFov(float fov) { m_fov = fov; }
	float getFov() const { return m_fov; }

private:
	// 摄像机属性
	Vector3 m_position;
	Vector3 m_front;
	Vector3 m_up;
	Vector3 m_right;
	Vector3 m_world_up;

	// 摄像机的角度
	float m_yaw;
	float m_pitch;

	// 摄像机的操作属性
	float m_movement_speed;
	float m_mouse_sensitivity;
	float m_fov;

private:
	void updateCameraVectors();
};

#endif // !CAMERA_H


