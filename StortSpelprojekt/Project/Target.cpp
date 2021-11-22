#include "Target.h"

Target::Target(const std::string& file, const Vector3& position, const Vector3& rotation, UINT ID)
	:Model(file, "Target"), hit(false)
{
	SetID(ID);

	SetPosition(position);
	SetRotation(rotation);
	UpdateMatrix();

	collider = std::make_shared<BoundingBox>();
	collider->SetPosition(position.x, position.y + 5, position.z);
	collider->SetRotation(rotation);
	collider->SetScale(1.0f, 1.5f, 1.0f);
	collider->Update();
	//More shit here to be consistent with target mesh
}