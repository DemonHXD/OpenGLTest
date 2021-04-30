#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "input.h"
#include "camera.h"
Input *Singleton<Input>::singleton = nullptr;

void Input::init(GLFWwindow *window, Camera *&camera)
{
	m_window = window;
	m_camera = camera;

	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_move_callback);
	glfwSetScrollCallback(m_window, mouse_scroll_callback);
}

void Input::keyProcessInput(float deltaTime)
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, true);
	}
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_camera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_camera->ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_camera->ProcessKeyboard(RIGHT, deltaTime);
	}
}

void Input::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{

	Input::get_singleton().on_framebuffer_size_callback(width, height);
}

void Input::mouse_move_callback(GLFWwindow *window, double xpos, double ypos)
{
	Input::get_singleton().on_move_callback(xpos, ypos);
}

void Input::mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	Input::get_singleton().on_scroll_callback(yoffset);
}

void Input::on_move_callback(double xpos, double ypos)
{
	if (m_firstMouse)
	{
		m_lastX = xpos;
		m_lastY = ypos;
		m_firstMouse = false;
	}

	float xoffset = xpos - m_lastX;
	float yoffset = m_lastY - ypos; // reversed since y-coordinates go from bottom to top
	m_lastX = xpos;
	m_lastY = ypos;

	m_camera->ProcessMouseMovement(xoffset, yoffset, true);
}

void Input::on_scroll_callback(double yoffset)
{
	m_camera->ProcessMouseScroll(yoffset);
}

void Input::on_framebuffer_size_callback(int width, int height)
{
	glViewport(0, 0, width, height);
}