#include <glad/glad.h>
#include "texture_manager.h"
#include "texture.h"
#include "shader.h"
#include <assert.h>
#include <iostream>
#include <string>
#include <iostream>
#include "../engine/engine.h"

TextureManager *Singleton<TextureManager>::singleton = nullptr;

void TextureManager::addLoadTexture(Shader *shader, unsigned int loadTextureCount, ...)
{
    Engine &engine = Engine::get_singleton();
    va_list arg;
    TextureShader *m_ts = new TextureShader();
    auto textureNames = shader->getTextureNames();
    if (textureNames.empty())
    {
        std::cout << "textureNames is empty......" << std::endl;
        return;
    }
    __crt_va_start(arg, loadTextureCount);
    for (int i = 0; i < loadTextureCount; i++)
    {
        const char *texturePath = __crt_va_arg(arg, char *);
        Texture *texture = new Texture();
        texture->setUniformName(textureNames[i]);
        assert(texture->load(engine.getAssetPathByName(texturePath), true));
        m_ts->textures.push_back(texture);
    }
    m_ts->shader = shader;
    m_map_ts.insert(std::pair<unsigned int, TextureShader *>(shader->getShadeID(), m_ts));
    __crt_va_end(arg);
}

void TextureManager::addLoadTexture(Shader *shader, std::vector<Texture *> textures)
{
    TextureShader *m_ts = new TextureShader();
    m_ts->textures = textures;
    m_ts->shader = shader;
    m_map_ts.insert(std::pair<unsigned int, TextureShader *>(shader->getShadeID(), m_ts));
}

//void TextureManager::renderTexturesByShaderId(unsigned int shaderID, std::vector<std::string> texturesName)
//{
//    TextureShader *value = m_map_ts[shaderID];
//    if (value->textures.empty())
//    {
//        std::cout << "texture render fail......" << std::endl;
//        return;
//    }
//
//    for (unsigned int index = 0; index < value->textures.size(); index++)
//    {
//        std::string uniformName;
//        if(texturesName.size() == 0) 
//        {
//            uniformName = value->textures[index]->getUniformName();
//        }else
//        {
//            uniformName = texturesName[index];
//        }
//        glActiveTexture(GL_TEXTURE0 + index);
//        value->shader->setInt(uniformName, index);
//		value->textures[index]->active();
//    }
//}