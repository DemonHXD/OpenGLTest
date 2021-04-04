#ifndef _SHADERUTILS_H_
#define _SHADERUTILS_H_
#include "../../common/math.h"
#include <string>

class ShaderUtils
{
	// uniform¹¤¾ßº¯Êý
	void setBool(unsigned int shaderID, const std::string &name, bool value) const;
	void setInt(unsigned int shaderID, const std::string &name, int value) const;
	void setFloat(unsigned int shaderID, const std::string &name, float value) const;
	void setMat2(unsigned int shaderID, const std::string &name, const Matrix2 &mat) const;
	void setMat3(unsigned int shaderID, const std::string &name, const Matrix3 &mat) const;
	void setMat4(unsigned int shaderID, const std::string &name, const Matrix4 &mat) const;
}; 
#endif // !_SHADERUTILS_H_





