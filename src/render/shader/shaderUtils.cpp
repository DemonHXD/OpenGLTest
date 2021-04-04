#include "shaderUtils.h"
#include <glad/glad.h>

void ShaderUtils::setBool(unsigned int shaderID, const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
}
void ShaderUtils::setInt(unsigned int shaderID, const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}
void ShaderUtils::setFloat(unsigned int shaderID, const std::string &name, float value) const
{
	glUniform4f(glGetUniformLocation(shaderID, name.c_str()), 0.0f, value, 0.0f, 1.0f);
}
void ShaderUtils::setMat2(unsigned int shaderID, const std::string &name, const Matrix2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderUtils::setMat3(unsigned int shaderID, const std::string &name, const Matrix3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void ShaderUtils::setMat4(unsigned int shaderID, const std::string &name, const Matrix4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}