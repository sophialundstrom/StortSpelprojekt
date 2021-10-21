#include "NPCHostile.h"


HostileNPC::HostileNPC(const std::string& file, std::vector<std::shared_ptr<Arrow>> hostileArrows, std::shared_ptr<Player> player)
	:NPC(file)
{
	this->arrows = hostileArrows;
    this->player = player;
}

HostileNPC::HostileNPC(const Model& model)
	:NPC(model)
{

}

void HostileNPC::Update()
{
    float lastClick = 0;

    if (Event::KeyIsPressed('L'))
    {
        if (Time::Get() - lastClick > 1.f)
        {
            Vector3 aimDir = player->GetPosition() - position;
            aimDir.Normalize();

            movementYRadiant = acos(aimDir.x * 0 + aimDir.z * 1);
	        if (aimDir.x < 0)
		        movementYRadiant *= -1;

            movementXRadiant = acos(aimDir.Dot(Vector3(0, 1, 0)) / aimDir.Length());

            int currentIndex = 0;
            bool isEnemyShootingArrow = false;
            while (currentIndex < arrows.size() -1 && isEnemyShootingArrow == false)
            {
                isEnemyShootingArrow = arrows.at(currentIndex)->Shoot(aimDir, position, {PI_DIV2 - movementXRadiant, movementYRadiant, 0});
                lastClick = Time::Get();
                currentIndex++;            
            }
        }
    }

    for (int i = 0; i < arrows.size(); i++)
    {
        arrows.at(i)->Update();
    }
	NPC::Update();
}

void HostileNPC::WeaponSlash()
{
	//Highly prototype only
	// need some kind of way to do a weapon slash in the future
}
