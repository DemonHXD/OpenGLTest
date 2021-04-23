#include <glad/glad.h>
#include "texture.h"
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>

Texture::Texture()
	: m_id(0),
	  m_width(0),
	  m_height(0),
	  m_type(GL_TEXTURE_2D)
{
}

/*
	设置贴图类型
*/
void Texture::bindTextureType(unsigned int textureType)
{
	m_type = textureType;
}

bool Texture::load(const char *imgPath, bool isGenMipMap)
{
	int width, height, nrChannels;
	unsigned char *data = stbi_load(imgPath, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cout << "Failed to load texture, texture path is " << imgPath << std::endl;
		return false;
	}

	glGenTextures(1, &m_id);
	glBindTexture(m_type, m_id);
	assert(nrChannels == 3 || nrChannels == 4);
	unsigned int format;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	glTexImage2D(m_type, 0, GL_RGB, width, width, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(m_type);
	stbi_image_free(data);

	if (isGenMipMap)
	{
		glTexParameteri(m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	m_width = width;
	m_height = height;
	return true;
}

void Texture::active()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

/*
	释放贴图资源
*/
void Texture::unload()
{
	if (!m_id)
	{
		return;
	}
	glDeleteTextures(1, &m_id);
	m_id = 0;
}