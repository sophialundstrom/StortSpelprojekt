#include "Arrow.h"

Arrow::Arrow()
	:Model("ArrowPivotFront", "ArrowPivotFront"), speed(150.f), lifeTime(10.f)
{
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
			SetPosition(position + (direction * speed * Time::GetDelta()));
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
	//collider->Update();
	rayCollider->Update();
}
