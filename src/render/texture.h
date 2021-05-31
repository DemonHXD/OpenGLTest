#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Texture
{
public:
	struct TextureObject{
		Texture* texture;
		std::string texture_uniform_name;
		unsigned int texture_render_type;
	};
public:
	Texture();

	unsigned int getTextureID() const { return m_id; }
	int getWidth() const { return m_width; };
	int getHeight() const { return m_height; };

	bool load(const char *imgPath, bool isGenMipMap);
	bool loadCubeMap(std::vector<std::string> faces);

	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

	void unload();

	void active(unsigned int renderType);

	void bindTextureType(unsigned int textureType);

	std::string type;
	std::string path;

private:
	unsigned int m_id;
	unsigned int m_type;
	int m_width;
	int m_height;
};
#endif // !_TEXTURE_H_
