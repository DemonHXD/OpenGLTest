#include "lib_utils.h"
#include "stdio.h"
#include <iostream>

LibUtils *LibUtils::pInstance = nullptr;

LibUtils *LibUtils::getInstance()
{
	if (nullptr == pInstance)
	{
		pInstance = new LibUtils();
	}
	return pInstance;
}

/*
	封装格式化字符串
*/
std::string LibUtils::formatString(const char *text, ...)
{
	va_list list;
	//1. 先获取格式化后字符串的长度
	__crt_va_start(list, text);
	int size = vsnprintf(NULL, 0, text, list);
	__crt_va_end(list);
	if (size <= 0)
	{
		return NULL;
	}
	size++;

	//2. 复位va_list，将格式化字符串写入到buf
	__crt_va_start(list, text);
	char *buf = (char *)malloc(size);
	vsnprintf(buf, size, text, list);
	__crt_va_end(list);
	return buf;
}

