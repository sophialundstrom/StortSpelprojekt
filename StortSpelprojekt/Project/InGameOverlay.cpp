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
	for (auto& [info, line] : lines)
	{
		if (line.progress < 1.0f)
		{
			line.progress += 2.0f * Time::GetDelta();
			if (line.progress > 1.0f)
				line.progress = 1.0f;
			line.p2.x = line.p1.x + line.progress * line.width;
		}
	}

	GetText("Interact")->Hide();
	buildingRequirements.Hide();

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

	for (auto& [info, line] : lines)
		UI::Inst().GetRenderTarget()->DrawLine(line.p1, line.p2, lineBrush, 2.0f);

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

	//INTERACT
	AddText({ (float)Window::ClientWidth() / 2.0f, (float)Window::ClientHeight() - 200.0f }, "Interact", "Interact [E]", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED, false);

	//UPGRADE
	AddText({ (float)Window::ClientWidth() / 2.0f, (float)Window::ClientHeight() - 400.0f }, "Requirements", "Requirements", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED, false);
	AddText({ (float)Window::ClientWidth() / 2.0f, (float)Window::ClientHeight() - 300.0f }, "StickRequirements", "", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED, false);
	AddText({ (float)Window::ClientWidth() / 2.0f, (float)Window::ClientHeight() - 200.0f }, "StoneRequirements", "", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED, false);
	AddText({ (float)Window::ClientWidth() / 2.0f, (float)Window::ClientHeight() - 100.0f }, "Upgrade", "", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED, false);
	buildingRequirements.requirements = GetText("Requirements");
	buildingRequirements.stickRequirement = GetText("StickRequirements");
	buildingRequirements.stoneRequirement = GetText("StoneRequirements");
	buildingRequirements.upgrade = GetText("Upgrade");

	//INVENTORY
	D2D_VECTOR_2F position = { Window::ClientWidth() - 500.0f, 100.0f };
	AddImage(position, "AASLOT", "InventorySlot.png");
	AddImage(position, "APPLE", "FOOD.png");
	AddText({ position.x, position.y + 100.0f }, "FOOD", "0", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED);

	position.x += 200.0f;
	AddImage(position, "AASTICK", "InventorySlot.png");
	AddImage(position, "STICK", "WOOD.png");
	AddText({ position.x, position.y + 100.0f }, "STICK", "0", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED);

	position.x += 200.0f;
	AddImage(position, "AASTONE", "InventorySlot.png");
	AddImage(position, "STONE", "STONE.png");
	AddText({ position.x, position.y + 100.0f }, "STONE", "0", UI::COLOR::YELLOW, UI::TEXTFORMAT::TITLE_CENTERED);

	//LINE BRUSH
	lineBrush = UI::Inst().GetBrush(UI::COLOR::BROWN);
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
				const std::string completeName = quest->GetName() + objective->Info() + std::to_string(objectives.size());
				objectives.emplace_back(new Text(to_wstr(objective->Info()), position, objectiveFormat, color));
				
				if (objective->IsCompleted() && lines.find(completeName) == lines.end())
				{
					Line line = {};
					line.width = objectives.back()->GetExactWidth();
					line.progress = 0.0f;
					line.p1 = { position.x, position.y - objectives.back()->GetHeight() / 2.0f };
					line.p2 = line.p1;
					lines[completeName] = line;
				}
			}
		}

		auto pair = std::make_pair(name, objectives);

		this->quests.emplace_back(pair);
	}

	if (!lines.empty())
	{
		for (UINT i = lines.size(); i > 0; i--)
		{
			auto ID = i - 1;
			auto line = std::next(lines.begin(), ID);

			Quest* foundQuest = nullptr;

			for (auto quest : quests)
			{
				UINT count = 0;
				for (auto objective : quest->GetObjectives())
				{
					if (quest->GetName() + objective->Info() + std::to_string(count) == line->first)
					{
						foundQuest = quest;
						break;
					}

					count++;
				}

				if (foundQuest)
					break;
			}

			if (!foundQuest || (foundQuest && foundQuest->IsCompleted()))
			{
				Print(line->first, "REMOVED");
				lines.erase(line->first);
			}
		}
	}
}

void InGameOverlay::UpdateInventory(Inventory& inventory)
{
	GetText("FOOD")->SetString(std::to_string(inventory.NumOf(Item::Type::Food)));
	GetText("STICK")->SetString(std::to_string(inventory.NumOf(Item::Type::Stick)));
	GetText("STONE")->SetString(std::to_string(inventory.NumOf(Item::Type::Stone)));
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

void InGameOverlay::ShowInteract()
{
	if (buildingRequirements.requirements->IsVisible())
		return;
	GetText("Interact")->Show();
}

void InGameOverlay::ShowUpgrade(Vector2 requirements, Vector2 requirementsCompleted, const std::string& completedCustomString)
{
	if (GetText("Interact")->IsVisible())
		return;

	buildingRequirements.Show();

	if (completedCustomString != "")
	{
		buildingRequirements.requirements->SetString(completedCustomString);
		buildingRequirements.stickRequirement->SetString("");
		buildingRequirements.stoneRequirement->SetString("");
		buildingRequirements.upgrade->SetString("");
		return;
	}

	buildingRequirements.requirements->SetString("Requirements");

	if (requirements.x == requirementsCompleted.x && requirements.y == requirementsCompleted.y)
	{
		buildingRequirements.stickRequirement->SetString("Sticks: " + std::to_string((UINT)requirementsCompleted.x) + " of " + std::to_string((UINT)requirements.x));
		buildingRequirements.stoneRequirement->SetString("Stone: " + std::to_string((UINT)requirementsCompleted.y) + " of " + std::to_string((UINT)requirements.y));
		buildingRequirements.upgrade->SetString("Press [E] To Upgrade");
	}

	else
	{
		buildingRequirements.stickRequirement->SetString("Sticks: " + std::to_string((UINT)requirementsCompleted.x) + " of " + std::to_string((UINT)requirements.x));
		buildingRequirements.stoneRequirement->SetString("Stone: " + std::to_string((UINT)requirementsCompleted.y) + " of " + std::to_string((UINT)requirements.y));
		buildingRequirements.upgrade->SetString("");
	}
}