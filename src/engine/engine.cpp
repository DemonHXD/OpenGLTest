#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <assert.h>

#include "engine.h"
#include "camera.h"
#include "file_system.h"
#include "input.h"

#include "../render/render.h"

Engine *Singleton<Engine>::singleton = nullptr;

Engine::Engine()
{
	initEngine();
}

void Engine::initEngine()
{
	m_camera = new Camera();

	// 初始化文件系统
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

	// 注册输入输出事件
	m_input->init(m_window, m_camera);

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
		m_input->keyProcessInput(deltaTime);

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