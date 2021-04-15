#ifndef _ENGINE_H
#define _ENGINE_H

#include "../common/singleton.h"
#include <vector>
#include <string>
#include <map>

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

	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&) = delete;

	bool initWindow(unsigned int width, unsigned int height);
	void run();
	void stopRun();

	float get_time() const;
	
public:

	GLFWwindow* getWindowIns() const {
		return m_window;
	}

	Camera* getCamera() const {
		return m_camera;
	}

	std::string getEnginePath() const;
	std::vector<std::string> getFilesBySuffix(std::string suffixName);

	const char* getAssetPathByName(std::string assetName) const
	{
		return m_assets.at(assetName).c_str();
	}

private:
	void initEngine();
	void preLoadAllAsset();
	void getFiles(std::string path, std::vector<std::string>& files);
	void keyProcessInput(float deltaTime);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void on_framebuffer_size_callback(int width, int height);
	void on_move_callback(double xpos, double ypos);
	void on_scroll_callback(double yoffset);

private:
	GLFWwindow* m_window = nullptr;
	Camera* m_camera;

	bool m_firstMouse = true;
	float m_lastX = 800.0f / 2.0;
	float m_lastY = 600.0 / 2.0;
	float m_deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float m_lastFrame = 0.0f; // 上一帧的时间

	std::map<std::string, std::string> m_assets;
};
#endif // !_ENGINE_H

