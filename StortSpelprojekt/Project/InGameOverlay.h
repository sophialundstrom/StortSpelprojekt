#pragma once
#include "Overlay.h"
#include "Inventory.h"
#include "Quest.h"

class InGameOverlay : public Overlay
{
private:
	std::vector<std::pair<Text*, std::vector<Text*>>> quests;

	void ClearQuests();
public:
	InGameOverlay();
	~InGameOverlay();

	void UpdateQuests(const std::vector<Quest*>& quests);
	void UpdateInventory(Inventory& inventory);
	void UpdateArrowCounter(UINT amount);
	void UpdateHealth(UINT amount);

	// Inherited via Overlay
	virtual void Render() override;
	virtual OVERLAYSTATE Update() override;
};