//#include "camera.h"
//
//void camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
//{
//	float velocity = MovementSpeed * deltaTime;
//	switch (direction)
//	{
//	case FORWARD:
//		Position += Front * velocity;
//		break;
//	case BACKWARD:
//		Position -= Front * velocity;
//		break;
//	case LEFT:
//		Position -= Right * velocity;
//		break;
//	case RIGHT:
//		Position += Right * velocity;
//		break;
//	}
//	Position.y = 0.0f;
//}
//
//void camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
//{
//	xoffset *= MouseSensitivity;
//	yoffset *= MouseSensitivity;
//
//	Yaw		+= xoffset;
//	Pitch	+= yoffset;
//
//	if (constrainPitch)
//	{
//		if (Pitch > 89.0f)
//		{
//			Pitch = 89.0f;
//		}
//		else if (Pitch < -89.0f)
//		{
//			Pitch = -89.0f;
//		}
//	}
//	updateCameraVectors();
//}
//
//void camera::ProcessMouseScroll(float yoffset)
//{
//	Zoom -= (float)yoffset;
//	if (Zoom < 1.0f)
//	{
//		Zoom = 1.0f;
//	}
//	else if (Zoom > 45.0f)
//	{
//		Zoom == 45.0f;
//	}
//}
//
//void camera::updateCameraVectors()
//{
//	Vec3 front;
//	front.x = cos(glm::radians(Yaw) * cos(glm::radians(Pitch)));
//	front.y = sin(glm::radians(Pitch));
//	front.z = sin(glm::radians(Yaw) * cos(glm::radians(Pitch)));
//	Front = glm::normalize(front);
//
//	Right = glm::normalize(glm::cross(Front, WorldUp));
//	Up = glm::normalize(glm::cross(Right, Front));
//}
