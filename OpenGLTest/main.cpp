#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	glfwInit();
	// 设置主版本和次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 使用核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口 设置窗口的宽高以及标题
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Faild to create GLFW window" << endl;

		glfwTerminate();
		return -1;
	}
	// 将窗口设置为当前的线程的上下文
	glfwMakeContextCurrent(window);

	// 初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// 设置渲染窗口(视口)的宽高
	glViewport(0, 0, 800, 600);

	// 注册每次调整窗口大小时触发的回调事件
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glfwWindowShouldClose 用来判断窗口是否被关闭，没有被关闭则一直进行渲染循环
	// 渲染循环
	while (!glfwWindowShouldClose(window)) {

		// 输入
		processInput(window);

		//渲染指令……
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// 渲染循环结束后我们需要正确释放/删除之前分配的资源
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}