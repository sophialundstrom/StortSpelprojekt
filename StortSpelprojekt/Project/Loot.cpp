#include "Loot.h"

Loot::Loot(LOOTTYPE type, const Vector3& position, const float& lifeTime)
{
	boundingBox = std::make_shared<BoundingBox>();
	this->lifeTime = lifeTime;
	switch (type)
	{
		case LOOTTYPE::MIXED:
		{
			ApplyMesh("Bird");
			ApplyMaterial("Bird");
		}
		case LOOTTYPE::ARROWS:
		{
			ApplyMesh("Arrow");
			ApplyMaterial("Arrow");
		}
	}
	SetPosition(position);
}

void Loot::Update()
{
	//rotation = rotation * Quaternion::CreateFromRotationMatrix(Matrix::CreateRotationY(rotationSpeed * Time::GetDelta()));
	currentLifeTime += Time::GetDelta();
	if (currentLifeTime > lifeTime)
		destroy = true;
}
