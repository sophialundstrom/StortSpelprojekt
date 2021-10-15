#include "Arrow.h"

Arrow::Arrow(const std::string file)
	:Model(file, "Arrow")
{
	speed = 10.f;
	lifeTime = 5.0f;
}

Arrow::~Arrow()
{
}

bool Arrow::Shoot(Vector3 direction, Vector3 startPos, Vector3 rotation)
{
	if(isShot == false)
	{
		xRadius = rotation.x;

		SetRotation({ rotation.x, rotation.y + PI, rotation.z});
		this->direction = direction;
		SetPosition(startPos);
		isShot = true;
	}
	
	return isShot;
}

void Arrow::Update()
{
	if (isShot == true)
	{
		lifeLength += Time::GetDelta();
	}

	if (lifeLength <= lifeTime)
	{
		if(rotation.x > -PI_DIV4)
		{
			rotation.x -= 0.05f * Time::GetDelta();
		}
		direction.y -= 0.05f * Time::GetDelta();
		SetPosition(GetPosition() + direction * speed * Time::GetDelta());
	}
	else
	{
		SetPosition(0, -100, 0);
		isShot = false;
		lifeLength = 0;
	}
	Model::Update();
}
