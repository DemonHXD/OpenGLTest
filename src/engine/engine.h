#ifndef _ENGINE_H
#define _ENGINE_H

#include "../common/singleton.h"
#include <vector>
#include <string>
#include <map>

class Input;
struct GLFWwindow;
class Camera;

//typedef void(*UpdateFun)();
class Engine : public Singleton<Engine>
{
public:

	struct _finddata_t
	{
		unsigned attrib;
		time_t time_create;
		time_t time_access;
		time_t time_write;
		_fsize_t size;
		char name[_MAX_FNAME];
	};

	Engine();
	~Engine() = default;

	Engine(const Engine &) = delete;
	Engine(Engine &&) = delete;
	Engine &operator=(const Engine &) = delete;
	Engine &operator=(Engine &&) = delete;

	
	bool initWindow(unsigned int width, unsigned int height);
	void run();
	void stopRun();

	float get_time() const;

public:

	GLFWwindow *getWindowIns() const
	{
		return m_window;
	}

	Camera *getCamera() const
	{
		return m_camera;
	}
private:

	void initEngine();

	GLFWwindow *m_window = nullptr;
	Camera *m_camera;
	std::shared_ptr<Input> m_input = std::make_shared<Input>();;

	float m_deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float m_lastFrame = 0.0f; // 上一帧的时间
};
#endif // !_ENGINE_H
