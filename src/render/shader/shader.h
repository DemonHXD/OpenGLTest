#ifndef _SHADER_H_
#define _SHADER_H_

class Shader
{
public:
	// 构造器读取并构造着色器
	Shader(const char* vertexPath, const char* fragmentPath);

	// 使用/激活着色器
	void use();

	unsigned int getShadeID() const
	{
		return shaderID;
	}

private:
	// 检查着色器是否创建成功
	void checkShaderCreate(unsigned int shader, const char* shaderTypeName);
	// 检查着色器程序是否创建成功
	void chackShaderProgramCreate();
	// 根据类型创建着色器
	unsigned int createShaderForType(const char* shaderSource, const char* shaderName, int shaderType);
	// 创建着色器程序
	unsigned int createShaderProgram(int shaderCount, ...);

private:
	unsigned int shaderID;
};
#endif // !_SHADER_H_

