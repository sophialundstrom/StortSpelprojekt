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
	lifeLength = lifeTime * 2;
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
		std::cout << "ARROW SHOT" << std::endl;
	}
	
	return isArrowActivating;
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
			//Rotate downwards
			rotation.x -= 0.05f * Time::GetDelta();
		}
		//Gravitation
		direction.y -= 0.05f * Time::GetDelta();
		SetPosition(GetPosition() + (direction * speed * Time::GetDelta()));
	}
	else
	{
		SetPosition(0, -100, 0);
		collider->SetPosition(collider->GetPosition().x, collider->GetPosition().y, collider->GetPosition().z - 0.5f);
		isShot = false;
		lifeLength = 0;
	}

	collider->Update();
	Model::Update();
}
