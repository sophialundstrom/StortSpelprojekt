#include "Objective.h"

Objective::Objective(Type type, bool completed)
	:type(type), completed(completed)
{
}

void Objective::WriteToFile(File& file)
{
	file.Write(type);
	file.Write(completed);
}

void Objective::ReadFromFile(File& file)
{
	file.Read(completed);
}
