#pragma once
#include "Objective.h"
#include "Math.h"

class LocationObjective : public Objective
{
private:
	Vector3 location;
	float radius;
public:
	LocationObjective(bool completed, Vector3 location, float radius);
	void Update(const Vector3& playerPosition);

	// Inherited via Objective
	virtual std::string Info() override;
};