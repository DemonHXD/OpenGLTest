#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <cassert>

template<class T>
class Singleton
{
public:
	Singleton()
	{
		assert(!singleton);
		singleton = static_cast<T*>(this);
	}
	~Singleton()
	{
		assert(singleton);
		singleton = nullptr;
	}
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static T& get_singleton() { return *singleton; }
	static T* get_singletonPtr() { return singleton; }

private:
	static T* singleton;
};

#endif // !_SINGLETON_H_
