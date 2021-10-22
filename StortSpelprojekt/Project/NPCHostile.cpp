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
     static float lastClick = 0;

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

            for (int i = 0; i < arrows.size(); i++)
            {
                if (arrows[i]->IsShot())
                    continue;

                arrows.at(i)->Shoot(aimDir, position, {PI_DIV2 - movementXRadiant, movementYRadiant, 0});
                lastClick = Time::Get();
                break;
            }
        }
    }

    for (int i = 0; i < arrows.size(); i++)
        arrows.at(i)->Update();

	NPC::Update();
}

void HostileNPC::WeaponSlash()
{
	//Highly prototype only
	// need some kind of way to do a weapon slash in the future
}
