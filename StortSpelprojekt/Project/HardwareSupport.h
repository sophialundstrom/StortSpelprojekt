#pragma once
#include <thread>

struct HardwareSupport
{
	static int numThreads;
};

inline int HardwareSupport::numThreads = std::thread::hardware_concurrency();