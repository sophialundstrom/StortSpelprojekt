#pragma once
#include <chrono>

struct Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
public:
	void Start()
	{
		start = std::chrono::steady_clock::now();
	}

	float DeltaTime()
	{
		auto end = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0f;
	}
};

class Time
{
private:
	static float value;
	static float delta;
public:
	static void Update(float dt) { value += dt; if (value >= FLT_MAX) value = 0; delta = dt; }
	static float Get() { return value; }
	static float GetDelta() { return delta; }
};

inline float Time::value = 0;
inline float Time::delta = 0;