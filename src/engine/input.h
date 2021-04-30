#ifndef _INPUT_H
#define _INPUT_H
#include "../common/singleton.h"

struct GLFWwindow;
class Camera;

class Input : public Singleton<Input>
{
public:
    void init(GLFWwindow *window, Camera* &camera);
    void keyProcessInput(float deltaTime);

private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void mouse_move_callback(GLFWwindow *window, double xpos, double ypos);
    static void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

    void on_framebuffer_size_callback(int width, int height);
    void on_move_callback(double xpos, double ypos);
    void on_scroll_callback(double yoffset);


    bool m_firstMouse = true;
    float m_lastX = 800.0f / 2.0;
    float m_lastY = 600.0 / 2.0;
    Camera* m_camera;
    GLFWwindow *m_window;
};
#endif