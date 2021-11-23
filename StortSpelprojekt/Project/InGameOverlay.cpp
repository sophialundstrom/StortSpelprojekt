#include "InGameOverlay.h"
#include "Window.h"

void InGameOverlay::ClearQuests()
{
	for (auto& pair : quests)
	{
		delete pair.first;
		for (auto objective : pair.second)
			delete objective;
	}

	quests.clear();
}

OVERLAYSTATE InGameOverlay::Update()
{
	returnState = OVERLAYSTATE::NO_CHANGE;

	if (Event::KeyIsPressed(VK_ESCAPE))
		returnState = OVERLAYSTATE::PAUSE;

	return returnState;
}

void InGameOverlay::Render()
{
	UI::Inst().BeginFrame();

	DrawImages();
	DrawTexts();

	for (auto& quest : quests)
	{
		quest.first->Draw();
		for (auto objective : quest.second)
			objective->Draw();
	}

	UI::Inst().EndFrame();
}

InGameOverlay::InGameOverlay()
{
	HideCursor();

	//QUESTS
	AddImage({ 250, 365 }, "QuestBorder", "QuestBorder.png");
	AddText({ 250, 65 }, "AQ", "Quests", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED);

	//HEALTH
	AddImage({ 355, Window::ClientHeight() - 64.0f }, "hp", "HP10.png");

	//FPS
	AddText({ (float)Window::ClientWidth() - 100, (float)Window::ClientHeight() - 30 }, "FPS", "FPS: 0", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_SMALL);

	//CROSSHAIR
	AddImage({ (float)Window::ClientWidth() / 2.0f, (float)Window::ClientHeight() / 2 }, "CrossHair", "CrossHair.png");

	//ARROWS
	AddText({ (float)Window::ClientWidth() / 2.0f, (float)Window::ClientHeight() - 50 }, "ArrowCount", "Arrows:" + std::to_string(0), UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED);
}

InGameOverlay::~InGameOverlay()
{
	ClearQuests();
}

void InGameOverlay::UpdateQuests(const std::vector<Quest*>& quests)
{
	ClearQuests();

	auto color = UI::Inst().GetBrush(UI::COLOR::YELLOW);
	auto questFormat = UI::TEXTFORMAT::TITLE_SMALL;
	auto objectiveFormat = UI::TEXTFORMAT::DEFAULT;

	D2D_VECTOR_2F position = { 70.0f, 60.0f };
	const UINT objectiveOffset = 30;
	const UINT questOffset = 60;

	for (UINT i = 0; i < quests.size(); ++i)
	{
		auto quest = quests[i];

		position.y += questOffset;

		auto name = new Text(to_wstr(quest->GetName()), position, questFormat, color);
		std::vector<Text*> objectives;

		if (quest->IsCompleted())
		{
			position.y += objectiveOffset;
			objectives.emplace_back(new Text(L"Return to " + to_wstr(quest->GetQuestHolder()) + L".", position, objectiveFormat, color));
		}

		else
		{
			for (auto objective : quest->GetObjectives())
			{
				position.y += objectiveOffset;
				objectives.emplace_back(new Text(to_wstr(objective->Info()), position, objectiveFormat, color));
			}
		}

		auto pair = std::make_pair(name, objectives);

		this->quests.emplace_back(pair);
	}
}

void InGameOverlay::UpdateInventory(Inventory& inventory)
{
	//UPDATE INVENTORY UI
}

void InGameOverlay::UpdateArrowCounter(UINT amount)
{
	UpdateText("ArrowCount", "Arrows: " + std::to_string(amount));
}

void InGameOverlay::UpdateHealth(UINT amount)
{
	auto image = GetImage("hp");
	if (image->FileName() == "HP" + std::to_string(amount) + ".png")
		return;

	auto position = image->GetPosition();
	RemoveImage("hp");
	AddImage(position, "hp", "HP" + std::to_string(amount) + ".png");
}

void InGameOverlay::UpdateFPS(UINT FPS)
{
	UpdateText("FPS", "FPS: " + std::to_string(FPS));
}