#include "MyWindow.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

MyWindow::MyWindow(unsigned int version, unsigned int width, unsigned int height)
{
	glfwInit();
	// 设置主版本和次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 使用核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口 设置窗口的宽高以及标题
	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Faild to create GLFW window" << std::endl;
		glfwTerminate();
	}
	// 将窗口设置为当前的线程的上下文
	glfwMakeContextCurrent(window);

	// 初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// 设置渲染窗口(视口)的宽高
	glViewport(0, 0, 800, 600);

	// 注册每次调整窗口大小时触发的回调事件
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

}

void MyWindow::setUpdateCallBack(UpdateFun callBack)
{
	//// 渲染循环
	//while (!glfwWindowShouldClose(window)) {
	//	// 输入
	//	processInput(window);

	//	if (callBack != NULL)
	//	{
	//		callBack();
	//	}

	//	// 检查并调用事件，交换缓冲
	//	glfwPollEvents();
	//	glfwSwapBuffers(window);
	//}

	//// 渲染循环结束后我们需要正确释放/删除之前分配的资源
	//glfwTerminate();
}

