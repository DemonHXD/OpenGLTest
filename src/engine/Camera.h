#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

typedef glm::vec3 Vec3;
typedef glm::mat4 Mat4;

const float YAW				= -90.f;
const float PITCH			= 0.0f;
const float SPEED			= 2.5f;
const float SENSITIVITY		= 0.1f;
const float ZOOM			= 45.0f;

class Camera
{
public:
	Camera(Vec3 psition = Vec3(0.0f, 0.0f, 3.0f), Vec3 up = Vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		:Front(Vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = psition;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		:Front(Vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = Vec3(posX, posY, posZ);
		WorldUp = Vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	Mat4 GetViewMatrix() const
	{
		return glm::lookAt(Position, Position + Front, Up);
	}
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
	void ProcessMouseScroll(float yoffset);

public:
	// 摄像机属性
	Vec3 Position;
	Vec3 Front;
	Vec3 Up;
	Vec3 Right;
	Vec3 WorldUp;

	// 摄像机的角度
	float Yaw;
	float Pitch;

	// 摄像机的操作属性
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

private:
	void updateCameraVectors();
};

#endif // !CAMERA_H


