#ifndef _TEXTURE_H_
#define _TEXTURE_H_
class Texture
{
public:

	Texture();

	unsigned int getTextureID() const { return m_textureID; }
	int getWidth() const { return m_width; };
	int getHeight() const { return m_height; };

	bool load(const char* imgPath, bool isGenMipMap);

	void unload();

	void bindTextureType(unsigned int textureType);
private:
	unsigned int m_textureID;
	unsigned int m_textureType;
	int m_width;
	int m_height;
};
#endif // !_TEXTURE_H_



