#include <glad/glad.h>
#include "texture_manager.h"
#include "texture.h"
#include "shader.h"
#include <assert.h>
#include <iostream>
#include <string>
#include "../engine/engine.h"

TextureManager *Singleton<TextureManager>::singleton = nullptr;

void TextureManager::addLoadTexture(unsigned int loadTextureCount, ...)
{
    Engine& engine = Engine::get_singleton();
    va_list arg;
    __crt_va_start(arg, loadTextureCount);
    for (int i = 0; i < loadTextureCount; i++)
    {
        const char * texturePath = __crt_va_arg(arg, char *);
        Texture *texture = new Texture();
        assert(texture->load(engine.getAssetPathByName(texturePath), true));
        m_textures.push_back(texture);
    }
    __crt_va_end(arg);
}

void TextureManager::renderAllTexture(Shader* shader)
{
    if (m_textures.empty())
    {
        std::cout << "texture render fail......" << std::endl;
        return;
    }

    int index = 0;
    for (Texture *texture : m_textures)
    {
        char str[20];
        sprintf_s(str, "texture%d", index + 1);
        shader->setInt(str, index);
        glActiveTexture(GL_TEXTURE0 + index);
        texture->active();
        index++;
    }
}