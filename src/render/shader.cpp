#include <glad/glad.h>
#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../engine/engine.h"

bool Shader::loadShaderAsset(const char *vertexAssetName, const char *fragmentAssetName)
{
	Engine &engine = Engine::get_singleton();
	const char *vertexPath = engine.getAssetPathByName(vertexAssetName);
	const char *fragmentPath = engine.getAssetPathByName(fragmentAssetName);
	// 1.从文件路径中获取顶点/片段着色器
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// 保证ifstream对象可以抛出异常
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		// 转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return false;
	}
	const char *vShaderCoder = vertexCode.c_str();
	const char *fShaderCoder = fragmentCode.c_str();
	unsigned int vertexShader, fragmentShader;
	vertexShader = createShaderForType(vShaderCoder, "vertexShader", GL_VERTEX_SHADER);
	fragmentShader = createShaderForType(fShaderCoder, "fragmentShader", GL_FRAGMENT_SHADER);
	createShaderProgram(2, vertexShader, fragmentShader);
	return true;
}

void Shader::chackShaderProgramCreate()
{
	int success;
	char infoLog[512];
	glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success);
	if (!success)
	{
		// 用于获取错误消息，存储在infoLog字符数组中
		glGetProgramInfoLog(m_shaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::shaderProgram::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
}

void Shader::checkShaderCreate(unsigned int shader, const char *shaderTypeName)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// 用于获取错误消息，存储在infoLog字符数组中
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shaderTypeName << "::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
}

unsigned int Shader::createShaderForType(const char *shaderSource, const char *shaderName, int shaderType)
{
	unsigned int shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	checkShaderCreate(shader, shaderName);
	return shader;
}

void Shader::createShaderProgram(int shaderCount, ...)
{
	// unsigned int shaderProgram;
	va_list arg;
	m_shaderID = glCreateProgram();
	__crt_va_start(arg, shaderCount);
	for (int i = 0; i < shaderCount; i++)
	{
		unsigned int shader = __crt_va_arg(arg, unsigned int);
		// 着色器程序绑定着色器
		glAttachShader(m_shaderID, shader);
		// 绑定完之后进行释放
		glDeleteShader(shader);
	}
	glLinkProgram(m_shaderID);
	__crt_va_end(arg);
	chackShaderProgramCreate();
}

void Shader::bind() const
{
	glUseProgram(m_shaderID);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::setTextureNames(unsigned int textureNameCount, ...)
{
	va_list arg;
	__crt_va_start(arg, textureNameCount);
	for (int i = 0; i < textureNameCount; i++)
	{
		const char *textureName = __crt_va_arg(arg, char *);
		m_textureName.push_back(std::string(textureName));
	}
	__crt_va_end(arg);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(m_shaderID, name.c_str()), value);
}
void Shader::setMat2(const std::string &name, const Matrix2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string &name, const Matrix3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string &name, const Matrix4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_shaderID, name.c_str()), x, y, z);
}
void Shader::setVec3(const std::string &name, const glm::vec3 pos) const
{
	glUniform3fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, &pos[0]);
}