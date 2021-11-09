#pragma once
#include "Model.h"
#include "Collision.h"
#include "Player.h"

enum class LOOTTYPE { MIXED, ARROWS };
class Loot : public Model
{
private:
	// SETTINGS
	float rotationSpeed = 20.f;
	float lifeTime = 10.f; // NOT IN SECOND. DON'T KNOW WHY THOUGHT...
	float currentLifeTime = 0.f;
	float maxPickupRange = 8.f;
	UINT maxItemsPerType = 5.f;

	// COLLISION
	std::shared_ptr<BoundingSphere> boundingSphere;
	bool destroy = false;

	// LOOTTYPE
	LOOTTYPE type;
	UINT numArrows = 0;
	UINT numStones = 0;
	UINT numFood = 0;
	UINT numWood = 0;

	void DealMixedItems();

public:
	Loot(LOOTTYPE type, const Vector3& position, const float& lifeTime);
	virtual void Update() override;
	void Update(std::shared_ptr< Player> player);
	const bool& IsDestroyed() { return this->destroy; }
	std::shared_ptr<BoundingSphere> GetCollider() { return boundingSphere; }

};