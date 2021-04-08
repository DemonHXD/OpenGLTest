#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "../common/singleton.h"
// #include <vector>
#include <map>

class Shader;

class ShaderManager : public Singleton<ShaderManager>
{
public:
    void addLoadShader(std::map<std::string, Shader*> shaders);

    std::map<std::string, Shader*> getShaders() const
    {
        return m_shaders;
    }

private:
    std::map<std::string, Shader*> m_shaders;
};

#endif