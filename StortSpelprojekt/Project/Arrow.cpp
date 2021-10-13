#include "Arrow.h"

Arrow::Arrow(const std::string file)
	:Model("Arrow", "Arrow")
{
	speed = 50.f;
	lifeTime = 5.0f;
}

Arrow::~Arrow()
{
}

bool Arrow::Shoot(Vector3 direction, Vector3 startPos, Vector3 rotation)
{
	//Vector3 rotReference(0, 0, 1);
	//float xRadian = Get2DAngle(rotReference.y, rotReference.z, direction.y, direction.z);
	//float yRadian = Get2DAngle(rotReference.x, rotReference.z, direction.x, direction.z);
	//float zRadian = Get2DAngle(rotReference.x, rotReference.y, direction.x, direction.y);

	SetRotation({ rotation.x, rotation.y + PI, rotation.z});
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

//float Get2DAngle(float a1, float a2, float b1, float b2)
//{
//	//MathExplanation
//	//https://stackoverflow.com/questions/42554960/get-xyz-angles-between-vectors
//
//
//	return acos(a1 * b1 + a2 * b2);
//};
