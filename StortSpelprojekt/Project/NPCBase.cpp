#include "NPCBase.h"

NPC::NPC(const std::string& file)
{
}

NPC::NPC()
{
}

void NPC::AddModel(std::map<std::string, std::shared_ptr<Drawable>>& drawables, const std::string& file)
{
	model = std::dynamic_pointer_cast<Model>(drawables[file]);
}


