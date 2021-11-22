#pragma once
#include <string>
#include "File.h"

class Objective
{
public:
	enum class Type { TALK, COLLECT, FIGHT, TARGET, LOCATION };
protected:
	Type type;
	bool completed;
public:
	Objective() = default;
	Objective(Type type, bool completed);

	void Complete() { completed = true; }
	bool IsCompleted() { return completed; }

	virtual std::string Info() = 0;
	virtual void WriteToFile(File& file);
	virtual void ReadFromFile(File& file);
};