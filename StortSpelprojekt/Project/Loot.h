#pragma once
#include "Model.h"
#include "Collision.h"

enum class LOOTTYPE { MIXED, ARROWS };
class Loot : public Model
{

private:
	float rotationSpeed = 2.f;
	float lifeTime = 5.f;
	float currentLifeTime = 0.f;
	std::shared_ptr<BoundingBox> boundingBox;
public:
	Loot(LOOTTYPE type, const Vector3& position, const float& lifeTime);
	virtual void Update() override;
	std::shared_ptr<BoundingBox> GetCollider() { return boundingBox; }
	bool destroy = false;

};