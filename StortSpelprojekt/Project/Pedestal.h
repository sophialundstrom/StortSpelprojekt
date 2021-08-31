#pragma once
#include "Diamond.h"

class Pedestal : public Model
{
private:
	bool used = false;
	DirectX::BoundingBox bbox;
public:
	Pedestal(Vector3 position)
		:Model("Pedestal", position)
	{
		bbox = DirectX::BoundingBox(position, Vector3{ bounds.Xmax - bounds.Xmin, bounds.Ymax - bounds.Ymin, bounds.Zmax - bounds.Zmin } *0.5f * scale);
	}

	void PlaceDiamond(std::shared_ptr<Diamond> diamond)
	{
		diamond->SetTargetPosition(Vector3(position.x, position.y + 3.0f, position.z));
		used = true;
	}

	bool IsUsed() const { return used; }

	const DirectX::BoundingBox& GetBounds() const { return this->bbox; }
};