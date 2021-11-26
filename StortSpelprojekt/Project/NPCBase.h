#pragma once
#include "Model.h"
#include "Collision.h"
#include "Player.h"
#include "Arrow.h"
#include "Pathfinding.h"
#include "AnimatedModel.h"

class NPC : public AnimatedModel
{
protected:
	int hp;
	int maxHP;
	bool dead = false;
	Vector3 spawnPosition;
protected:
	std::shared_ptr<BoundingBox> boundingBox;
public:
	NPC(const std::string& file);
	NPC(const AnimatedModel& model);

	virtual void Update() override;
	bool Collided(Player &player);
	
	void Die();
	bool IsDead() { return dead; }
	void TakeDamage();
	std::shared_ptr<BoundingBox> GetCollider() { return boundingBox; }
	void debugPrint();
};