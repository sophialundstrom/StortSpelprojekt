#include "NPCFriendly.h"

#include "QuestLog.h"

FriendlyNPC::FriendlyNPC(const std::string& name, const std::string& file, int health)
	:NPC(file, health), currentDialogueState(DialogueState::HANDOUT)
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

	static bool walking = false;
	if (Event::KeyIsPressed('G'))
		walking = true;
	else if (Event::KeyIsPressed('H'))
		walking = false;
	if(walking)
		Walking();

	if (dialogueOverride)
	{
		questMarker->SetPosition(position.x, position.y + 12.0f, position.z);
		questMarker->SetAsObjective();
		return;
	}

	UINT completedQuests = 0;
	if (currentQuest)
	{
		if (!currentQuest->Unlocked())
		{
			questMarker->SetPosition(0, -1000.0f, 0);
			return;
		}

		questMarker->SetPosition(position.x, position.y + 12.0f,position.z);

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

void FriendlyNPC::Walking()
{
	//TODO: Implement walking behaviour
	//static int pathIndex = 0;
	//if (Vector3::Distance(position, player->GetPosition()) > 4.0f)
	//{
	//	if (pathIndex < pathing->GetGrid()->GetPath().size())
	//	{
	//		moveDirection = pathing->GetGrid()->GetPath()[pathIndex] - position;
	//		moveDirection.Normalize();

	//		position += (moveDirection * speed * Time::GetDelta());

	//		if (Vector3::Distance(pathing->GetGrid()->GetPath()[pathIndex], position) < 0.2f)
	//		{
	//			pathIndex++;

	//		}
	//	}
	//	else
	//	{
	//		pathIndex = 0;
	//		if (Vector3::Distance(pathing->GetGrid()->GetPosition(), position) > 8.0f)
	//		{
	//			//pathing->CreateGrid(Vector3((int)position.x, (int)position.y, (int)position.z));
	//		}
	//		pathing->FindPath(position, player->GetPosition());
	//		return;
	//	}
	//}
	//else // delete path because we want to stop before collision 
	//{
	//	pathing->GetGrid()->GetPathRef().clear();
	//	//path.clear();
	//}
}