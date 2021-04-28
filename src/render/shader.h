#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <vector>
#include <map>
#include "texture.h"
#include "../common/math.h"

class Shader
{
public:

	Shader() = default;
	~Shader() = default;

	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;

	Shader& operator=(const Shader&) = delete;
	Shader& operator=(Shader&&) = delete;

	bool loadShaderAsset(const char* vertexAssetName, const char* fragmentAssetName);
	void renderTextures(std::vector<std::string> texturesName, std::vector<Texture*> textures);
	void renderTextures();
	void renderTexture(std::string textureName, std::string uniformName);
	void bind() const;
	void unbind() const;

	unsigned int getShadeID() const
	{
		return m_shaderID;
	}

	void setTexturesName(unsigned int textureNameCount, ...);
	void setTextures(unsigned int texturesCount, ...);

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat2(const std::string &name, const Matrix2 &mat) const;
	void setMat3(const std::string &name, const Matrix3 &mat) const;
	void setMat4(const std::string &name, const Matrix4 &mat) const;
	void setVec3(const std::string &name, float x, float y, float z)const;
	void setVec3(const std::string &name, const glm::vec3 pos) const;

private:
	// 检查着色器是否创建成功
	void checkShaderCreate(unsigned int shader, const char* shaderTypeName);
	// 检查着色器程序是否创建成功
	void chackShaderProgramCreate();
	// 根据类型创建着色器
	unsigned int createShaderForType(const char* shaderSource, const char* shaderName, int shaderType);
	// 创建着色器程序
	void createShaderProgram(int shaderCount, ...);
private:
	unsigned int m_shaderID;
	std::vector<std::string> m_texturesName;
	std::vector<Texture*> m_textures;
	std::map<std::string, Texture*> m_map_texture;
};
#endif // !_SHADER_H_

