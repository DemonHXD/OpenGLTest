#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "../common/singleton.h"
#include <vector>

class Texture;
class Shader;
class TextureManager : public Singleton<TextureManager>
{
public:
    void addLoadTexture(unsigned int loadTextureCount, ...);

    void renderAllTexture(Shader* shader);
private:
    std::vector<Texture*> m_textures;
};

#endif