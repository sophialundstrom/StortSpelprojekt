#include "Loot.h"
#include "Random.h"
#include <cmath>
#include <algorithm>

void Loot::DealMixedItems()
{
	numFood = Random::Integer(1, maxItemsPerType - 2);
	numWood = Random::Integer(1, maxItemsPerType - 2);
	numStones = Random::Integer(1, maxItemsPerType - 2);
	numArrows = Random::Integer(1, maxItemsPerType - 2);
}

Loot::Loot(LOOTTYPE inType, const Vector3& position)
{
	boundingSphere = std::make_shared<BoundingSphere>();
	switch (inType)
	{
		case LOOTTYPE::MIXED:
		{
			this->type = inType;
			ApplyMesh("Barb");
			ApplyMaterial("Barb");
			SetScale(0.4, 0.4, 0.4);
			DealMixedItems();
			break;
		}
		case LOOTTYPE::ARROWS:
		{
			this->type = inType;
			ApplyMesh("arrowModel");
			ApplyMaterial("arrowModel");
			SetScale(2, 2, 2);
			numArrows = Random::Integer(3, maxItemsPerType + 10);
			break;
		}
		default:
			break;
	}

	SetPosition(position);
	originalPosition = position;
	boundingSphere->SetPosition(position);
	boundingSphere->SetScale(8);
}

void Loot::Update()
{
	Model::Update();
	boundingSphere->Update();
}

void Loot::Update(std::shared_ptr<Player> player)
{
	Update();
	currentRotation += Time::GetDelta();
	if (!isTaken)
	{
		float sin = std::sin(currentRotation * floatingSpeed) * 2;
		SetPosition(originalPosition + Vector3(0, sin, 0));
	}
	rotation = Quaternion::CreateFromRotationMatrix(Matrix::CreateRotationY(currentRotation * rotationSpeed));
	currentLifeTime += Time::GetDelta();

	if (currentLifeTime > lifeTime)
	{
		std::cout << "Destroyed by time\n";
		destroy = true;
	}

	float distance = (player->GetPosition() - GetPosition()).Length();

	if (distance < maxPickupRange)
	{

		if (Collision::Intersection(boundingSphere, player->GetBounds()) || isTaken)
		{
			isTaken = true;
			float posX = std::lerp(position.x, player->GetPosition().x, lerpSpeed);
			float posY = std::lerp(position.y, player->GetPosition().y, lerpSpeed);
			float posZ = std::lerp(position.z, player->GetPosition().z, lerpSpeed);
			SetPosition(posX, posY, posZ);

			if (distance < collectRange)
			{

				if (type == LOOTTYPE::MIXED)
				{
				for (int wood = 0; wood < numWood; wood++)
					player->Inventory().AddItem(RESOURCE::WOOD);
				for (int stone = 0; stone < numStones; stone++)
					player->Inventory().AddItem(RESOURCE::STONE);
				for (int food = 0; food < numFood; food++)
					player->Inventory().AddItem(RESOURCE::FOOD);
				}
				// GIVE ARROWS HERE....
				player->numArrows += numArrows;

				std::cout << "Destroyed by collision\n";
				destroy = true;
			}
		}
	}

}
