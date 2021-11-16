#include "Arrow.h"

Arrow::Arrow()
	:Model("arrowModel", "arrowModel"), speed(150.f), lifeTime(100.f)
{
	collider = std::make_shared<BoundingBox>();
	rayCollider = std::make_shared<RayCollider>();
}

void Arrow::Update()
{
	lastPosition = position;
	lifeLength += Time::GetDelta();

	if (lifeLength <= lifeTime){

		if (!isStuck)
		{
			direction.y -= 0.05f * Time::GetDelta();
			SetPosition(GetPosition() + (direction * speed * Time::GetDelta()));
			rayCollider->origin = lastPosition;
			rayCollider->direction = position - lastPosition;
			rayCollider->length = rayCollider->direction.Length();
			rayCollider->direction.Normalize();
		}
	}
	else
	{
		isDestroyed = true;
	}

	Model::Update();
	collider->Update();
	rayCollider->Update();
}
