#include "NPCFriendly.h"

#include "QuestLog.h"

FriendlyNPC::FriendlyNPC(const std::string& name, const std::string& file)
	:NPC(file), currentDialogueState(DialogueState::HANDOUT)
{
	SetName(name);
	questMarker = std::make_shared<QuestMarker>();
	boundingBox->SetScale(2, 4, 2);
}

Quest* FriendlyNPC::AddQuest(const std::string& name)
{
	auto quest = QuestLog::Get(name);
	if (quest)
	{
		quest->SetQuestHolder(this->name);
		quests.emplace_back(quest);
		return quest;
	}
	
	return nullptr;
}

void FriendlyNPC::AddDialogue(const std::string& string)
{
	dialogues.emplace_back(string);
}

bool FriendlyNPC::Interactable()
{
	if (completedAllQuests || dialogueOverride)
		return true;

	if (!currentQuest)
		return false;

	return currentQuest->Unlocked();
}

void FriendlyNPC::ActivateCurrentQuest()
{
	QuestLog::Activate(currentQuest);
}

const std::string FriendlyNPC::GetCurrentDialogue()
{
	if (!completedAllQuests)
	{
		auto ID = (3 * currentQuestID) + UINT(currentDialogueState);
		return dialogues[ID];
	}

	else
		return dialogues.back();
}

void FriendlyNPC::Update()
{
	NPC::Update();

	questMarker->Update();

	if (dialogueOverride)
	{
		questMarker->SetPosition(0, 9.0f, 0);
		questMarker->SetAsObjective();
		return;
	}

	if (currentQuest)
	{
		if (!currentQuest->Unlocked())
		{
			questMarker->SetPosition(0, -1000.0f, 0);
			return;
		}

		questMarker->SetPosition(0, 9.0f, 0);

		if (currentQuest->Unlocked() && currentQuest->IsActive() && !currentQuest->IsCompleted())
		{
			currentDialogueState = DialogueState::HELP;
			questMarker->SetAsHelp();
		}
			
		else if (currentQuest->IsCompleted())
		{
			currentDialogueState = DialogueState::HANDIN;
			questMarker->SetAsDone();

			if (finishedDialogue)
			{
				QuestLog::Complete(currentQuest);

				currentQuestID++;
				currentQuest = nullptr;

				if (currentQuestID == quests.size())
				{
					completedAllQuests = true;
					currentDialogueState = DialogueState::DONE;
					questMarker->SetAsComplete();
				}
			}
		}

		else
		{
			questMarker->SetAsGive();
			currentDialogueState = DialogueState::HANDOUT;
		}
	}

	if (!currentQuest && !completedAllQuests)
	{
		currentQuest = quests[currentQuestID];
		questMarker->SetAsGive();
	}

	finishedDialogue = false;
}