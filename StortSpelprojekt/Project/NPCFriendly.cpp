#include "NPCFriendly.h"

FriendlyNPC::FriendlyNPC(const std::string& file)
	: NPC(file)
{
	questMarker = std::make_shared<QuestMarker>();
	boundingBox->SetScale(2, 4, 2);
}

FriendlyNPC::FriendlyNPC(const Model& model)
	: NPC(model)
{
}

void FriendlyNPC::Update()
{
	NPC::Update();

	activeQuestID = -1;
	interactable = false;
	Vector3 qmPosition = { 0, -1000, 0 };
	Walking();
	UINT completedQuests = 0;
	for (auto& ID : questIDs)
	{
		if (QuestLog::Inst().QuestIsActive(ID))
		{
			activeQuestID = ID;
			interactable = true;
			qmPosition = { 0, 10, 0 };
			break;
		}

		if (QuestLog::Inst().QuestIsDone(ID))
			completedQuests++;
	}

	if (completedQuests >= questIDs.size())
		completed = true;
	
	questMarker->SetPosition(qmPosition);
	questMarker->Update();
}

void FriendlyNPC::Walking()
{
	//TODO: Implement walking behaviour
	static int pathIndex = 0;
	if (pathIndex < path.size())
	{
		std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
		SetPosition(path[pathIndex]);
		std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
	}
	pathIndex++;
}