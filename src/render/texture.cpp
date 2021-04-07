#include <glad/glad.h>
#include "texture.h"
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>

Texture::Texture()
	:m_textureID(0),
	m_width(0),
	m_height(0),
	m_textureType(GL_TEXTURE_2D)
{

}

/*
	设置贴图类型
*/
void Texture::bindTextureType(unsigned int textureType)
{
	m_textureType = textureType;
}

bool Texture::load(const char* imgPath, bool isGenMipMap)
{
	int width, height, nrChannels;
	unsigned char *data = stbi_load(imgPath, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cout << "Failed to load texture, texture path is " << imgPath << std::endl;
		return false;
	}

	glGenTextures(1, &m_textureID);
	glBindTexture(m_textureType, m_textureID);
	assert(nrChannels == 3 || nrChannels == 4);
	const unsigned int format = nrChannels == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(m_textureType, 0, GL_RGB, width, width, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(m_textureType);
	stbi_image_free(data);

	if (isGenMipMap)
	{
		glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	m_width = width;
	m_height = height;
	return true;
}

/*
	释放贴图资源
*/
void Texture::unload()
{
	if (!m_textureID)
	{
		return ;
	}
	glDeleteTextures(1, &m_textureID);
	m_textureID = 0;
}