#pragma once
#include "Model.h"
#include "Collision.h"
#include "Player.h"
#include "Arrow.h"

class NPC : public Model
{
private:
	int hp;
	bool dead = false;
protected:
	std::shared_ptr<BoundingBox> boundingBox;
public:
	NPC(const std::string& file);
	NPC(const Model& model);

	virtual void Update() override;
	bool Collided(Player &player);
	bool ProjectileCollided(std::shared_ptr<Arrow>& arrow);
	void Die();
	bool IsDead() { return dead; }
	void TakeDamage();
	std::shared_ptr<BoundingBox> GetCollider() { return boundingBox; }
	//void AddModel(std::map<std::string, std::shared_ptr<Drawable>> &drawables, const std::string& file);
	void debugPrint();
};