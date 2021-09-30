#include "NPCBase.h"


NPC::NPC(const std::string& file)
	:Model(file)
{
}

void NPC::Update()
{
	//UpdateMatrix();
	Model::Update();
	//debugPrint();
	//this->position += Vector3(0.002f, 0, 0);
	Print(position.x);
}

void NPC::debugPrint()
{
	Print("NPC action");
}
