#pragma once
#include "Model.h"
#include "Collision.h"

class Arrow : public Model
{
private:
	float speed;
	float lifeTime;
	float lifeLength = 0;
	bool isShot = false;
	Vector3 direction;
	float xRadius = 0;
	std::shared_ptr<BoundingBox> collider;


public:
	bool isStuck = false;
	Arrow();
	~Arrow();

	void DisableArrow();
	bool Shoot(Vector3 direction, Vector3 startPos, Vector3 rotation);
	std::shared_ptr<BoundingBox> GetCollider() { return collider; }
	bool IsShot() { return isShot; }
	void Update();
};