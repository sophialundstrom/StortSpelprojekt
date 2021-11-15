#include "Arrow.h"

Arrow::Arrow()
	:Model("arrowModel", "arrowModel"), speed(150.f), lifeTime(4.f)
{
	collider = std::make_shared<BoundingBox>();
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
		}
	}
	else
	{
		isDestroyed = true;
	}

	Model::Update();
	collider->Update();
}
