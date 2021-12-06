#include "LocationObjective.h"

LocationObjective::LocationObjective(const Vector3& location, float radius)
	:Objective(Type::LOCATION, false), location(location), radius(radius) {}

void LocationObjective::Update(const Vector3& playerPosition)
{
	if ((playerPosition - location).Length() < radius)
		Complete();
}

std::string LocationObjective::Info()
{
	return "Travel To Marked Location.";
}

void LocationObjective::WriteToFile(File& file)
{
	Objective::WriteToFile(file);

	file.Write(location);
	file.Write(radius);
}

void LocationObjective::ReadFromFile(File& file)
{
	this->type = Objective::Type::LOCATION;

	Objective::ReadFromFile(file);

	file.Read(location);
	file.Read(radius);
}
