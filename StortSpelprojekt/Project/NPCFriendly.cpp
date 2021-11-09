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
	///{
	static bool walking = false;
	if (Event::KeyIsPressed('G'))
		walking = true;

	if(walking)
		Walking();

	//}
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
	if (Vector3::Distance(position, player->GetPosition()) > 1.0f)
	{
		if (pathIndex < pathing->GetGrid()->GetPath().size())
		{
			Vector3 moveDirection = pathing->GetGrid()->GetPath()[pathIndex] - position;
			moveDirection.Normalize();

			position += (moveDirection * speed * Time::GetDelta());

			if (Vector3::Distance(pathing->GetGrid()->GetPath()[pathIndex], position) < 0.2f)
			{
				pathIndex++;

			}
		//	std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
		////	SetPosition(pathing->GetGrid()->GetPath()[pathIndex]);
		//	std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;

		}
		else
		{
			pathIndex = 0;
			if (Vector3::Distance(pathing->GetGrid()->GetPosition(), position) > 8.0f)
			{
				//pathing->CreateGrid(position);
			}
			pathing->FindPath(position, player->GetPosition());
			return;
		}
	}
}

void FriendlyNPC::SetPlayerPtr(std::shared_ptr<Player> p)
{
	player = p;
}
