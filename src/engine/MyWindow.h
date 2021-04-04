#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>

typedef void(*UpdateFun)();
class MyWindow
{
public:
	MyWindow(unsigned int version, unsigned int width, unsigned int height);
	void setUpdateCallBack(UpdateFun callBack);
	GLFWwindow* getWindowIns() const {
		return window;
	}

private:
	GLFWwindow* window;
};
#endif // !MY_WINDOW_H

