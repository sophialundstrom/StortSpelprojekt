#include "Loot.h"

Loot::Loot(LOOTTYPE type, const Vector3& position, const float& lifeTime)
{
	boundingBox = std::make_shared<BoundingBox>();
	this->lifeTime = lifeTime;
	switch (type)
	{
		case LOOTTYPE::MIXED:
		{
			ApplyMesh("pole1");
			ApplyMaterial("pole1");
			SetScale(1, 1, 1);
		}
		case LOOTTYPE::ARROWS:
		{
			ApplyMesh("Arrow");
			ApplyMaterial("Arrow");
			SetScale(1, 1, 1);

		}
		default:
			break;
	}
	SetPosition(position);
}

void Loot::Update()
{
	rotationSpeed += Time::GetDelta();
	rotation = Quaternion::CreateFromRotationMatrix(Matrix::CreateRotationY(rotationSpeed));
	currentLifeTime += Time::GetDelta();
	if (currentLifeTime > lifeTime)
		destroy = true;
	Model::Update();
	boundingBox->Update();
}
