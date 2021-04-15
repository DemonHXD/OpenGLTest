#ifndef _LIB_UTILS_H_
#define _LIB_UTILS_H_
#include <string>
class LibUtils {
public:
	static LibUtils* getInstance();
	std::string formatString(const char* text, ...);
private:
	LibUtils() {};
	static LibUtils* pInstance;
};
#endif // !_LIB_UTILS_H_
