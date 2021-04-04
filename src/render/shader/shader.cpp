#include <glad/glad.h>
#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
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
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCoder = vertexCode.c_str();
	const char* fShaderCoder = fragmentCode.c_str();
	unsigned int vertexShader, fragmentShader;
	vertexShader = createShaderForType(vShaderCoder, "vertexShader", GL_VERTEX_SHADER);
	fragmentShader = createShaderForType(fShaderCoder, "fragmentShader", GL_FRAGMENT_SHADER);
	shaderID = createShaderProgram(2, vertexShader, fragmentShader);
}

void Shader::chackShaderProgramCreate()
{
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
	if (!success) {
		// 用于获取错误消息，存储在infoLog字符数组中
		glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::shaderProgram::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::checkShaderCreate(unsigned int shader, const char* shaderTypeName)
{
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		// 用于获取错误消息，存储在infoLog字符数组中
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shaderTypeName << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

unsigned int Shader::createShaderForType(const char* shaderSource, const char* shaderName, int shaderType)
{
	unsigned int shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	checkShaderCreate(shader, shaderName);
	return shader;
}

unsigned int Shader::createShaderProgram(int shaderCount, ...)
{
	unsigned int shaderProgram;
	va_list arg;
	shaderProgram = glCreateProgram();
	__crt_va_start(arg, shaderCount);
	for (int i = 0; i < shaderCount; i++) {
		unsigned int shader = __crt_va_arg(arg, unsigned int);
		// 着色器程序绑定着色器
		glAttachShader(shaderProgram, shader);
		// 绑定完之后进行释放
		glDeleteShader(shader);
	}
	glLinkProgram(shaderProgram);
	__crt_va_end(arg);
	chackShaderProgramCreate();
	return shaderProgram;
}

void Shader::use()
{
	glUseProgram(shaderID);
}