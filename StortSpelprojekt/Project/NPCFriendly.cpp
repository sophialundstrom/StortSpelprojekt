#include "NPCFriendly.h"

FriendlyNPC::FriendlyNPC(const std::string& file)
	: NPC(file)
{
	boundingBox->SetScale(2, 4, 2);
	timer.Start();
}

FriendlyNPC::FriendlyNPC(const Model& model)
	: NPC(model)
{
	timer.Start();
}

void FriendlyNPC::Update()
{
	NPC::Update();

	activeQuestID = -1;
	interactable = false;

	
	std::cout << timer.DeltaTime() << std::endl;;
	timeMeasurement += timer.DeltaTime();
	if (timeMeasurement >= 2)
	{
		timer.Start();
		Walking();
		timeMeasurement = 0;
	}

	UINT completedQuests = 0;
	for (auto& ID : questIDs)
	{
		if (QuestLog::Inst().QuestIsActive(ID))
		{
			activeQuestID = ID;
			interactable = true;
			return;
		}

		if (QuestLog::Inst().QuestIsDone(ID))
			completedQuests++;
	}

	if (completedQuests >= questIDs.size())
		completed = true;
}

void FriendlyNPC::Walking()
{
	//TODO: Implement walking behaviour
	static int pathIndex = 0;
	if (pathIndex < path.size())
	{
		std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
		SetPosition(path[pathIndex]->position);
		std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
	}
	pathIndex++;
}