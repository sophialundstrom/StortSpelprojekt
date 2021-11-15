#pragma once
#include "Objective.h"
#include "Inventory.h"

class CollectObjective : public Objective
{
private:
	UINT numCollected;
	UINT numToCollect;
	Item::Type itemType;
public:
	CollectObjective() = default;
	CollectObjective(Item::Type type, UINT amount);
	void Update(Inventory& inventory);

	// Inherited via Objective
	virtual std::string Info() override;
	virtual void WriteToFile(File& file) override;
	virtual void ReadFromFile(File& file) override;
};