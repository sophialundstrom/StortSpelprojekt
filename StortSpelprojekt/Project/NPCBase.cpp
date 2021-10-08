#include "NPCBase.h"


NPC::NPC(const std::string& file)
	:Model(file, file)
{
	// call bind here cause i think it binds the bounding volume to a useful place
	boundingSphere.Bind();
}

NPC::NPC(const Model& model)
	: Model(model)
{
	boundingSphere.Bind();
}

void NPC::Update()
{
	boundingSphere.SetPosition(position);
	boundingSphere.Update(); // not sure what update does but it deos not seem to update the posiution of the boundingVolume
	Model::Update();
}

bool NPC::Collided(Player& player)
{
	// use this function to prototype the player interacting with a NPC
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
