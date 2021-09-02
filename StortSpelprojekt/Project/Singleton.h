#pragma once
#include "Print.h"

template <typename S>
class Singleton
{
public:
	static S& Inst() { if (!instance) Print("SINGLETON WAS NOT INSTANCED"); return *instance; }
protected:
	Singleton(S* s) { if (instance != nullptr) Print("SINGLETON WAS ALREADY INSTANCED"); instance = s; }
	~Singleton()    {  instance = nullptr; }
private:
	static S* instance;
	Singleton(Singleton const& other) = delete;
	Singleton(Singleton&& other) = delete;
};

template <typename S>
inline S* Singleton<S>::instance = nullptr;