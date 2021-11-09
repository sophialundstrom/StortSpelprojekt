#pragma once
#include "Model.h"
#include "Collision.h"
#include "Player.h"

enum class LOOTTYPE { MIXED, ARROWS };
class Loot : public Model
{
private:
	// SETTINGS
	float currentRotation = 0.f; // CURRENT ROTATION OF THE ITEM
	float rotationSpeed = 4.f; // SPEED OF THE ROTATION OF THE ITEM
	float lifeTime = 1000.f; // TIME UNTIL DESPAWNED. NOT IN SECONDS. DON'T KNOW WHY THOUGHT...
	float currentLifeTime = 0.f; // START LIFETIME
	float maxPickupRange = 9.f; // MAX RANGE OF LERP REGISTERING
	float collectRange = 4.2f; // MAX RANGE OF ITEM GETTING COLLECTED
	UINT maxItemsPerType = 5.f; // MAX ITEMS PER TYPE (USED FOR MIXED LOOT TYPE)
	float lerpSpeed = 0.12f; // LERP SPEED BETWEEN CURRENT POSITION AND PLAYER POSITION
	bool isTaken = false; // IF HAS BEEN TRIGGERED TO BE COLLECTED
	Vector3 originalPosition;
	float floatingSpeed = 2.f;

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
	Loot(LOOTTYPE type, const Vector3& position);
	virtual void Update() override;
	void Update(std::shared_ptr< Player> player);
	const bool& IsDestroyed() { return this->destroy; }
	std::shared_ptr<BoundingSphere> GetCollider() { return boundingSphere; }

};