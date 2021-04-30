#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <assert.h>

#include "engine.h"
#include "camera.h"
#include "file_system.h"

#include "../common/math.h"
#include "../render/render.h"

Engine *Singleton<Engine>::singleton = nullptr;

Engine::Engine()
{
	initEngine();
}

void Engine::initEngine()
{
	m_camera = new Camera();
	FileSystem::getInstance()->init();
}

bool Engine::initWindow(unsigned int width, unsigned int height)
{
	glfwInit();
	// 设置主版本和次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 使用核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口 设置窗口的宽高以及标题
	m_window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (m_window == NULL)
	{
		std::cout << "Faild to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 将窗口设置为当前的线程的上下文
	glfwMakeContextCurrent(m_window);

	// 初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// 设置渲染窗口(视口)的宽高
	glViewport(0, 0, width, height);

	// 注册每次调整窗口大小时触发的回调事件
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_move_callback);
	glfwSetScrollCallback(m_window, mouse_scroll_callback);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	// 开启混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void Engine::run()
{
	Render &render = Render::get_singleton();
	// 渲染循环
	while (!glfwWindowShouldClose(m_window))
	{

		const float currentFrame = glfwGetTime();
		const float deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;
		keyProcessInput(deltaTime);

		render.draw(deltaTime);

		// 检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}
}

void Engine::stopRun()
{
	// 渲染循环结束后我们需要正确释放/删除之前分配的资源
	glfwTerminate();
}

float Engine::get_time() const
{
	return glfwGetTime();
}

void Engine::keyProcessInput(float deltaTime)
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

void Engine::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{

	Engine::get_singleton().on_framebuffer_size_callback(width, height);
}

void Engine::mouse_move_callback(GLFWwindow *window, double xpos, double ypos)
{
	Engine::get_singleton().on_move_callback(xpos, ypos);
}

void Engine::mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	Engine::get_singleton().on_scroll_callback(yoffset);
}

void Engine::on_framebuffer_size_callback(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Engine::on_move_callback(double xpos, double ypos)
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

void Engine::on_scroll_callback(double yoffset)
{
	m_camera->ProcessMouseScroll(yoffset);
}
