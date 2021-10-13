#include "Arrow.h"

Arrow::Arrow(const std::string file)
	:Model("Arrow", "Arrow")
{
	speed = 10.0f;
	lifeTime = 5.0f;
}

Arrow::~Arrow()
{
}

bool Arrow::Shoot(Vector3 direction, Vector3 startPos)
{
	std::cout << "Arrow shot" << std::endl;
	this->direction = direction * speed;
	SetPosition(startPos);

	return true;
}

void Arrow::Update()
{
	SetPosition(GetPosition() + direction * Time::GetDelta());
	Model::Update();
}
