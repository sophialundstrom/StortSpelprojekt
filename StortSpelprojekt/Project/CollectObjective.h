#pragma once
#include "Objective.h"
#include "Inventory.h"

class CollectObjective : public Objective
{
private:
	UINT numCollected;
	UINT numToCollect;
	Item::Type type;
public:
	void Update(Inventory& inventory);

	// Inherited via Objective
	virtual std::string Info() override;
};