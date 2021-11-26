#pragma once
#include <thread>

struct HardwareSupport
{
	static int numThreads;
};

// NUMBER OF THREADS, ON THE CPU THAT IS USED, THAT CAN WORK IN PARALLELL.
inline int HardwareSupport::numThreads = std::thread::hardware_concurrency();