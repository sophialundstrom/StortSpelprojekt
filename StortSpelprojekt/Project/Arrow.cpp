#include "Arrow.h"

Arrow::Arrow(const std::string file)
	:Model(file, "Arrow"), speed(100.0f), lifeTime(5.0f)
{
	collider = std::make_shared<BoundingBox>();
}

Arrow::~Arrow()
{
}

void Arrow::DisableArrow()
{
	SetPosition(0, -200, 0);
	isShot = false;
	lifeLength = 0;
}

bool Arrow::Shoot(Vector3 direction, Vector3 startPos, Vector3 rotation)
{
	bool isArrowActivating = false;

	if(isShot == false)
	{
		SetRotation({ rotation.x, rotation.y + PI, rotation.z});
		this->direction = direction;
		SetPosition(startPos);
		isShot = true;
		isArrowActivating = true;
	}
	
	return isArrowActivating;
}

void Arrow::Update()
{
	if (isShot == true)
	{
		lifeLength += Time::GetDelta();
	}

	if (lifeLength <= lifeTime && isShot == true)
	{
		if(rotation.x > -PI_DIV4)
		{
			//Rotate downwards
			rotation.x -= 0.05f * Time::GetDelta();
		}
		//Gravitation
		direction.y -= 0.05f * Time::GetDelta();
		SetPosition(GetPosition() + (direction * speed * Time::GetDelta()));
	}
	else
	{
		DisableArrow();
	}

	collider->Update();
	Model::Update();
}
