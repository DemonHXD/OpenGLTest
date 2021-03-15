#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Window/MyWindow.h"
#include "Shader/Shader.h"
#include <iostream>
using namespace std;

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	MyWindow myWindow(3, 800, 600);
	GLFWwindow* wndIns = myWindow.getWindowIns();
	Shader shader("E:/openGL/openGL_C++_Pro/OpenGLTest/作业Code/五彩斑斓的三角形/shaderFile/vertexShader.vs",
		"E:/openGL/openGL_C++_Pro/OpenGLTest/作业Code/五彩斑斓的三角形/shaderFile/fragmentShader.fs");
	float vertices[] = {
			// 位置              // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO);

	 //渲染循环
	while (!glfwWindowShouldClose(wndIns)) {

		// 输入
		processInput(wndIns);

		//渲染指令……
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		float timeValue = glfwGetTime();
		float value = sin(timeValue) / 4.0f + 0.5f;
		shader.setFloat("updateColor", value);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// 检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(wndIns);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader.deleteShader();

	// 渲染循环结束后我们需要正确释放/删除之前分配的资源
	glfwTerminate();

	return 0;
}