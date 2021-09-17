#pragma once
#include "Model.h"
#include <map>

class NPC
{
protected:

	//Transform t;
	std::shared_ptr<Model> model;
public:
	NPC(const std::string& file);
	NPC();

	void AddModel(std::map<std::string, std::shared_ptr<Drawable>> &drawables, const std::string& file);
};