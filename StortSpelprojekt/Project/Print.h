#pragma once
#include <iostream>
#include <string>
#include "Math.h"

template <typename T>
inline void Print(T input, const std::string& source = "")
{
	if (source != "")
		std::cout << source + " >> ";
	std::cout << input << std::endl;
}

inline void PrintVector2(const Vector2& vector, const std::string& str = "")
{
	std::cout << str  << vector.x << " " << vector.y << std::endl;
}

inline void PrintVector3(const Vector3& vector, const std::string& str = "")
{
	std::cout << str  << vector.x << " " << vector.y << " " << vector.z << std::endl;
}

inline void PrintVector4(const Vector4& vector, const std::string& str = "")
{
	std::cout << str << vector.x << " " << vector.y << " " << vector.z << " " << vector.w << std::endl;
}

template <typename T>
inline void PrintNumber(const T& value, const std::string& str = "")
{
	std::cout << str << value << std::endl;
}

inline void PrintS(const std::string& str)
{
	std::cout << str << std::endl;
}