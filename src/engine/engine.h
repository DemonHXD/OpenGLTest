#ifndef _ENGINE_H
#define _ENGINE_H


#include "../common/singleton.h"
#include "input.h"

struct GLFWwindow;

//typedef void(*UpdateFun)();
class Engine : public Input
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

	void processInput(GLFWwindow* window) override;

	GLFWwindow* getWindowIns() const {
		return m_window;
	}

private:
	GLFWwindow* m_window = nullptr;
};
#endif // !_ENGINE_H

