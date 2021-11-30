#include "NPCBase.h"


NPC::NPC(const std::string& file)
	:Model(file, file)
{
	// call bind here cause i think it binds the bounding volume to a useful place
	boundingBox = std::make_shared<BoundingBox>();
	hp = maxHP = 3;
}

NPC::NPC(const Model& model)
	: Model(model)
{
	boundingBox = std::make_shared<BoundingBox>();
}

void NPC::Update()
{
	Model::Update();
	boundingBox->Update();
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

//bool NPC::ProjectileCollided(std::shared_ptr<Arrow>& arrow)
//{
//
//	if (Collision::Intersection(this->boundingBox, arrow->GetCollider()) && arrow->IsShot() == true)
//	{
//		Print("ARROW HIT");
//		//arrow->DisableArrow();
//		hp--;
//		if (hp <= 0)
//		{
//			Die();
//		}
//		return true;
//	}
//	return false;
//}

void NPC::Die()
{
	dead = true;
	//position = { 0,-100,0 };
}

void NPC::TakeDamage(int x)
{
	//std::cout << "DamageTaken\n";
	hp -= x;
	Print("Damage ");
	Print(x);
	if (hp <= 0)
		Die();
}

void NPC::debugPrint()
{
	Print("NPC action");
}
