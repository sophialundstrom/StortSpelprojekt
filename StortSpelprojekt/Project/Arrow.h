#pragma once
#include "Model.h"
#include "Collision.h"
#include "ModelRenderer.h"
#include "ColliderRenderer.h"

class Arrow : public Model
{
private:
	float speed;
	float lifeTime;
	float xRadius = 0;
	std::shared_ptr<BoundingBox> collider;


public:
	float lifeLength = 0;
	bool isDestroyed = false;
	bool isShot = true;
	bool canCollide = true;
	Vector3 direction;
	bool isStuck = false;
	Vector3 lastPosition;
	Arrow();
	~Arrow();

	//void DisableArrow();
	//bool Shoot(Vector3 direction, Vector3 startPos, Vector3 rotation);
	std::shared_ptr<BoundingBox> GetCollider() { return collider; }
	bool IsShot() { return isShot; }
	void Update();
};