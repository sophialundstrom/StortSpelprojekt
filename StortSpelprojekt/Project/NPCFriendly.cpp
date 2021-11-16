#include "NPCFriendly.h"

#include "QuestLog.h"

FriendlyNPC::FriendlyNPC(const std::string& name, const std::string& file)
	:NPC(file), currentDialogueState(DialogueState::HANDOUT)
{
	SetName(name);
	questMarker = std::make_shared<QuestMarker>();
	boundingBox->SetScale(2, 4, 2);
}

const std::string FriendlyNPC::AddQuest(const std::string& name)
{
	auto quest = QuestLog::Get(name);
	if (quest)
	{
		quests.emplace_back(quest);
		return quest->GetName();
	}
	
	return "";
}

void FriendlyNPC::AddDialogue(const std::string& string)
{
	dialogues.emplace_back(string);
}

bool FriendlyNPC::Interactable()
{
	if (!currentQuest)
		return false;

	return currentQuest->Unlocked();
}

void FriendlyNPC::ActivateCurrentQuest()
{
	QuestLog::Activate(currentQuest);
}

void FriendlyNPC::Update()
{
	NPC::Update();

	if (currentQuest)
	{
		if (!currentQuest->Unlocked())
			return;

		Print(currentQuest->GetName(), "========");

		if (currentQuest->Unlocked() && currentQuest->IsActive() && !currentQuest->IsCompleted())
		{
			Print("HELP");
			currentDialogueState = DialogueState::HELP;
		}

		else if (currentQuest->IsCompleted())
		{
			Print("HANDIN");
			currentDialogueState = DialogueState::HANDIN;
			if (finishedDialogue)
			{
				QuestLog::Complete(currentQuest);
				currentQuestID++;
				currentQuest = nullptr;
				return;
			}
		}

		else
		{
			Print("HANDOUT");
			currentDialogueState = DialogueState::HANDOUT;
		}

		questMarker->SetPosition(position.x, position.y + 5.0f, position.z);
		questMarker->Update();
	}

	if (!currentQuest)
	{
		currentQuest = quests[currentQuestID];
	}
}