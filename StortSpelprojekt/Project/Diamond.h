#pragma once
#include "Model.h"
#include "Camera.h"
#include "Time.h"

class Diamond : public Model
{
private:
	bool picked = false;
	bool used = false;
	DirectX::BoundingBox bbox;

	Vector3 targetPosition;
	float offset = 1.0f;
	float speed = 30.0f;
public:
	Diamond(Vector3 position)
		:Model("Diamond", position)
	{
		bbox = DirectX::BoundingBox(position, Vector3{ bounds.Xmax - bounds.Xmin, bounds.Ymax - bounds.Ymin, bounds.Zmax - bounds.Zmin } *0.5f * scale);
		this->targetPosition = position;
	}

	void PickUp() { picked = true; }

	void Update(const Camera& camera)
	{
		if (picked && !used)
			targetPosition = (camera.GetPosition() + camera.GetDirection() * offset) - Vector3(0, 1.5, 0);

		if ((targetPosition - position).Length() < 0.1f)
			return;

		Vector3 direction = targetPosition - position;
		direction.Normalize();

		Vector3 distance = direction * Time::GetDelta() * speed;
		if ((position - distance).Length() < 0.1f)
			position = targetPosition;
		else
			position += distance;

		bbox.Center = position;
	}

	bool IsUsed() { return this->used; }
	void SetTargetPosition(Vector3 position) { this->targetPosition = position; this->used = true; }
	const DirectX::BoundingBox& GetBounds() const { return this->bbox; }
};