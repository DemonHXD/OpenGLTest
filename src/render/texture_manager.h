#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "../common/singleton.h"
#include <vector>
#include <map>

class Texture;
class Shader;
class TextureManager : public Singleton<TextureManager>
{

public:
    struct TextureShader
    {
        Shader* shader;
        std::vector<Texture *> textures;
    };

public:
    void addLoadTexture(Shader* shader, unsigned int loadTextureCount, ...);
    void addLoadTexture(Shader* shader, std::vector<Texture *> mapTextures);

    //void renderTexturesByShaderId(unsigned int shaderID, std::vector<std::string> texturesName = std::vector<std::string>());
private:
    // std::vector<Texture*> m_textures;
    // TextureShader m_ts;
    std::map<unsigned int, TextureShader *> m_map_ts;
};

#endif