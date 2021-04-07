#include <glad/glad.h>
#include "Engine.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Engine::initEngine()
{
	
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
	m_window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (m_window == NULL) {
		std::cout << "Faild to create GLFW window" << std::endl;
		glfwTerminate();
	}
	// 将窗口设置为当前的线程的上下文
	glfwMakeContextCurrent(m_window);

	// 初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// 设置渲染窗口(视口)的宽高
	glViewport(0, 0, width, height);

	// 注册每次调整窗口大小时触发的回调事件
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	return true;
}

void Engine::run()
{
	// 渲染循环
	while (!glfwWindowShouldClose(m_window)) {

		//渲染指令……
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}
}

void Engine::deleteRes()
{
	// 渲染循环结束后我们需要正确释放/删除之前分配的资源
	glfwTerminate();
}

