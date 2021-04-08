#include "shader_manager.h"
#include <iostream>
#include <assert.h>
ShaderManager *Singleton<ShaderManager>::singleton = nullptr;

void ShaderManager::addLoadShader(std::map<std::string, Shader*> shaders)
{
    std::map<std::string, Shader*>::iterator it;
    for(it = shaders.begin(); it != shaders.end(); it++)
    {
        m_shaders.insert(std::pair<std::string, Shader*>(it->first, it->second));
    }
}