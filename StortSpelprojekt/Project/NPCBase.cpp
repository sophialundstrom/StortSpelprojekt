#include "NPCBase.h"


NPC::NPC(const std::string& file)
	:Model(file)
{
	boundingSphere.Bind();
}

void NPC::Update()
{
	boundingSphere.SetPosition(position);
	boundingSphere.Update();
	//UpdateMatrix();
	Model::Update();
	//debugPrint();
	//this->position += Vector3(0.002f, 0, 0);
	//Print("x:");
	//Print(boundingSphere.GetPosition().x);
	//Print(", y:");
	//Print(boundingSphere.GetPosition().y);
	//Print(", z:");
	//Print(boundingSphere.GetPosition().z);
}

bool NPC::Collided(Player& player)
{
	if ((position - player.GetPosition()).Length() < 8.0f)
	{
		Print("collided");
	}
	return false;
}

void NPC::debugPrint()
{
	Print("NPC action");
}
