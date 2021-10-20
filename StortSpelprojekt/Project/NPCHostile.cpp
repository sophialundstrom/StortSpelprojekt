#include "NPCHostile.h"


HostileNPC::HostileNPC(const std::string& file, std::vector<std::shared_ptr<Arrow>> hostileArrows)
	:NPC(file)
{
	this->arrows = hostileArrows;
}

HostileNPC::HostileNPC(const Model& model)
	:NPC(model)
{

}

void HostileNPC::Update()
{
    float lastClick = 0;

    if (Time::Get() - lastClick > 0.2f)
    {
        if (Event::KeyIsPressed('L'))
        {
            int currentIndex = 0;
            bool isPlayerShootingArrow = false;
            while (currentIndex < arrows.size() /* -1 /Remove last error for balanced speed but stupid*/ && isPlayerShootingArrow == false)
            {
                isPlayerShootingArrow = arrows.at(currentIndex)->Shoot({ 0, 1, 0 }, position, {1, 0, 0});
                lastClick = Time::Get();
                currentIndex++;
            }
            std::cout << "SHOT" << std::endl;
        }
    }
	NPC::Update();
}

void HostileNPC::WeaponSlash()
{
	//Highly prototype only
	// need some kind of way to do a weapon slash in the future
}
