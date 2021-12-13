#pragma once
#include "Overlay.h"
#include "Inventory.h"
#include "Quest.h"

class InGameOverlay : public Overlay
{
private:
	std::vector<std::pair<Text*, std::vector<Text*>>> quests;
	
	struct Line
	{
		D2D_POINT_2F p1, p2;
		float width;
		float progress;
	};
	std::map<std::string, Line> lines;
	ID2D1SolidColorBrush* lineBrush;

	struct BuildingRequirements
	{
		Text* requirements;
		Text* stickRequirement;
		Text* stoneRequirement;
		Text* upgrade;

		void Show() { requirements->Show(); stickRequirement->Show(); stoneRequirement->Show(); upgrade->Show(); }
		void Hide() { requirements->Hide(); stickRequirement->Hide(); stoneRequirement->Hide(); upgrade->Hide(); }
	} buildingRequirements;

	void ClearQuests();
public:
	InGameOverlay();
	~InGameOverlay();

	void UpdateQuests(const std::vector<Quest*>& quests);
	void UpdateInventory(Inventory& inventory);
	void UpdateArrowCounter(UINT amount);
	void UpdateHealth(UINT amount);
	void UpdateFPS(UINT FPS);

	void ShowInteract();
	void ShowUpgrade(Vector2 requirements, Vector2 requirementsCompleted, const std::string& completedCustomString = "");

	// Inherited via Overlay
	virtual void Render() override;
	virtual OVERLAYSTATE Update() override;
};