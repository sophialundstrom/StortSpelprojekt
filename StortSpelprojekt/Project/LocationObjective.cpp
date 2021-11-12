#include "LocationObjective.h"

LocationObjective::LocationObjective(bool completed, Vector3 location, float radius)
	:Objective(Type::LOCATION, completed), location(location), radius(radius) {}

void LocationObjective::Update(const Vector3& playerPosition)
{
	if ((playerPosition - location).Length() < radius)
		completed = true;
}

std::string LocationObjective::Info()
{
	return "Travel To Marked Location.";
}
