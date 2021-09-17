#pragma once
#include <memory>
#include <vector>
#include "Player.h"

class Quest
{
private:
	unsigned int ID;
protected:
	std::string name;
	bool done = false;
	UINT numTriggerQuests;
	std::vector<UINT> triggerQuests;
public:
	Quest() = default;

	void Complete()
	{
		done = true;
	}

	virtual void Activate() = 0;
	virtual void Update() = 0;
	virtual void RenderUI() = 0;
};