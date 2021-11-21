#pragma once
#include "Model.h"
#include "Collision.h"
#include "ModelRenderer.h"
#include "ColliderRenderer.h"

class Arrow : public Model
{
private:
	float lifeTime;
	float xRadius = 0;

public:
	float speed;
	float lifeLength = 0;
	bool isDestroyed = false;
	bool isShot = true;
	bool canCollide = true;
	Vector3 direction;
	bool isStuck = false;
	Vector3 lastPosition;
	std::shared_ptr<RayCollider> rayCollider;
	Arrow();
	~Arrow() {};

	void Update();
};