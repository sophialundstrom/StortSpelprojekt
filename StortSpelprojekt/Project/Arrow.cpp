#include "Arrow.h"

Arrow::Arrow()
	:Model("arrowModel", "arrowModel"), speed(150.f), lifeTime(4.f)
{
	collider = std::make_shared<BoundingBox>();
}

Arrow::~Arrow()
{
}

//void Arrow::DisableArrow()
//{
//	SetPosition(0, -200, 0);
//	isShot = false;
//	lifeLength = 0;
//	collider->Update();
//	Model::Update();
//}

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
