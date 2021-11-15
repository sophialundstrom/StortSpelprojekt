#pragma once
#include "Objective.h"
#include "Math.h"

class LocationObjective : public Objective
{
private:
	Vector3 location;
	float radius;
public:
	LocationObjective() = default;
	LocationObjective(const Vector3& location, float radius);
	void Update(const Vector3& playerPosition);

	// Inherited via Objective
	virtual std::string Info() override;
	virtual void WriteToFile(File& file) override;
	virtual void ReadFromFile(File& file) override;
};