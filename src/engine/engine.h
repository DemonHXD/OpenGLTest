#ifndef _ENGINE_H
#define _ENGINE_H


#include "../common/singleton.h"

struct GLFWwindow;
class Input;

//typedef void(*UpdateFun)();
class Engine
{
public:
	Engine() = default;
	~Engine() = default;

	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) = delete;

	void initEngine();
	bool initWindow(unsigned int width, unsigned int height);
	void run();
	void deleteRes();

	GLFWwindow* getWindowIns() const {
		return m_window;
	}

private:
	GLFWwindow* m_window = nullptr;
	Input* m_input = nullptr;
};
#endif // !_ENGINE_H

