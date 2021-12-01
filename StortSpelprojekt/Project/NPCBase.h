#pragma once
#include "Model.h"
#include "Collision.h"
#include "Player.h"
#include "Arrow.h"
#include "Pathfinding.h"

class NPC : public Model
{
protected:
	int hp;
	int maxHP;
	bool dead = false;
	Vector3 spawnPosition;
protected:
	std::shared_ptr<BoundingBox> boundingBox;
public:
	NPC(const std::string& file, int health);
	NPC(const Model& model, int health);

	virtual void Update() override;
	bool Collided(Player &player);
	
	void Die();
	bool IsDead() { return dead; }
	void TakeDamage(int x);
	std::shared_ptr<BoundingBox> GetCollider() { return boundingBox; }
	void debugPrint();
};