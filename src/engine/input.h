#ifndef _INPUT_H_
#define _INPUT_H_

struct GLFWwindow;

class Input
{
public:
	Input(GLFWwindow* window);
	~Input() = default;

	void openListen();

private:
	GLFWwindow* m_window;
};

#endif