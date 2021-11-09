#include "Loot.h"
#include "Random.h"

void Loot::DealMixedItems()
{
	numFood = Random::Integer(1, maxItemsPerType - 2);
	numWood = Random::Integer(1, maxItemsPerType - 2);
	numStones = Random::Integer(1, maxItemsPerType - 2);
	numArrows = Random::Integer(1, maxItemsPerType - 2);
}

Loot::Loot(LOOTTYPE type, const Vector3& position, const float& lifeTime)
{
	boundingSphere = std::make_shared<BoundingSphere>();
	this->lifeTime = lifeTime;
	switch (type)
	{
		case LOOTTYPE::MIXED:
		{
			type = LOOTTYPE::MIXED;
			ApplyMesh("pole1");
			ApplyMaterial("pole1");
			SetScale(1, 1, 1);
			DealMixedItems();
		}
		case LOOTTYPE::ARROWS:
		{
			type = LOOTTYPE::ARROWS;
			ApplyMesh("Arrow");
			ApplyMaterial("Arrow");
			SetScale(2, 2, 2);
			numArrows = Random::Integer(1, maxItemsPerType);
		}
		default:
			break;
	}

	SetPosition(position);
	boundingSphere->SetPosition(position);
}

void Loot::Update()
{
	Model::Update();
	boundingSphere->Update();
}

void Loot::Update(std::shared_ptr<Player> player)
{
	rotationSpeed += Time::GetDelta();
	rotation = Quaternion::CreateFromRotationMatrix(Matrix::CreateRotationY(rotationSpeed));
	currentLifeTime += Time::GetDelta();
	if (currentLifeTime > lifeTime)
	{
		std::cout << "Destroyed by time\n";
		destroy = true;
	}

	float distance = (player->GetPosition() - position).Length();
	//std::cout << "distance: " << distance << std::endl;
	distance = (player->GetBounds()->GetPosition() - position).Length();
	//std::cout << "bound distance: " << distance << std::endl;
	if ((player->GetPosition() - position).Length() < maxPickupRange)
	{
		if (Collision::Intersection(boundingSphere, player->GetBounds()))
		{
			/*if (type == LOOTTYPE::MIXED) // DOESN'T WORK TO DO THIS FOR SOME REASON
			{*/
				for (int wood = 0; wood < numWood; wood++)
					player->Inventory().AddItem(RESOURCE::WOOD);
				for (int stone = 0; stone < numStones; stone++)
					player->Inventory().AddItem(RESOURCE::STONE);
				for (int food = 0; food < numFood; food++)
					player->Inventory().AddItem(RESOURCE::FOOD);
			//}
			// GIVE ARROWS HERE....

			std::cout << "Destroyed by collision\n";
			destroy = true;
		}
	}
	Update();
}
