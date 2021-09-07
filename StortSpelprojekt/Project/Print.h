#pragma once
#include <iostream>
#include <string>

template <typename T>
inline void Print(T input, const std::string& source = "")
{
	if (source != "")
		std::cout << source + " >> ";
	std::cout << input << std::endl;
}