#include "Loot.h"

Loot::Loot(LOOTTYPE type, const Vector3& position, const float& lifeTime)
{
	boundingBox = std::make_shared<BoundingBox>();
	this->lifeTime = lifeTime;
	switch (type)
	{
		case LOOTTYPE::MIXED:
		{
			ApplyMesh("BlacksmithDoneTriangulated");
			ApplyMaterial("BlacksmithDoneTriangulated");
			SetScale(10, 10, 10);
		}
		case LOOTTYPE::ARROWS:
		{
			ApplyMesh("BlacksmithDoneTriangulated");
			ApplyMaterial("BlacksmithDoneTriangulated");
		}
		default:
			break;
	}
	SetPosition(position);
}

void Loot::Update()
{
	//rotation = rotation * Quaternion::CreateFromRotationMatrix(Matrix::CreateRotationY(rotationSpeed * Time::GetDelta()));
	currentLifeTime += Time::GetDelta();
	if (currentLifeTime > lifeTime)
		destroy = true;
	Model::Update();
	boundingBox->Update();
}
