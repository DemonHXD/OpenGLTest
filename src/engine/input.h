#ifndef _INPUT_H_
#define _INPUT_H_

struct GLFWwindow;

class Input
{
public:
	virtual void processInput(GLFWwindow* window);
	virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif