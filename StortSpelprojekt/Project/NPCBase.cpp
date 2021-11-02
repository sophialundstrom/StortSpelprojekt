#include "NPCBase.h"


NPC::NPC(const std::string& file)
	:Model(file, file)
{
	// call bind here cause i think it binds the bounding volume to a useful place
	boundingSphere = std::make_shared<BoundingSphere>();
	hp = 3;
}

NPC::NPC(const Model& model)
	: Model(model)
{
	boundingSphere = std::make_shared<BoundingSphere>();
}

void NPC::Update()
{
	boundingSphere->SetPosition(position);
	Model::Update();
	boundingSphere->Update();
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

bool NPC::ProjectileCollided(std::shared_ptr<Arrow>& arrow)
{
	if (Collision::Intersection(this->boundingSphere, arrow->GetCollider()) && arrow->IsShot() == true)
	{
		Print("ARROW HIT");
		arrow->DisableArrow();
		hp--;
		if (hp <= 0)
		{
			Die();
		}
		return true;
	}
	return false;
}

void NPC::Die()
{
	position = { 0,-100,0 };
}

void NPC::debugPrint()
{
	Print("NPC action");
}
