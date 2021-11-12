#pragma once
#include <string>

class Objective
{
public:
	enum class Type { TALK, COLLECT, FIGHT, TARGET, LOCATION };
protected:
	Type type;
	bool completed;
public:
	Objective(Type type, bool completed);

	bool IsCompleted() { return completed; }

	virtual std::string Info() = 0;
};