#include "Player.h"

void Player::CalcHeight(HeightMap* heightMap)
{
	const int lowX = (int)std::floor(position.x);
	const int highX = (int)std::ceil(position.x);
	const float Xdecimal = position.x - lowX;

	const int lowZ = (int)std::floor(position.z);
	const int highZ = (int)std::ceil(position.z);
	const float Zdecimal = position.z - lowZ;

	const float H1 = heightMap->data.at(Vector2((float)lowX, (float)lowZ)) * (1 - Xdecimal) * (1 - Zdecimal);
	const float H2 = heightMap->data.at(Vector2((float)highX, (float)highZ)) * Xdecimal * Zdecimal;
	const float H3 = heightMap->data.at(Vector2((float)lowX, (float)highZ)) * (1 - Xdecimal) * Zdecimal;
	const float H4 = heightMap->data.at(Vector2((float)highX, (float)lowZ)) * Xdecimal * (1 - Zdecimal);

	heightMapGroundLevel = position.y = H1 + H2 + H3 + H4;
}

float Get2DAngle(Vector2 a, Vector2 b)
{
	//MathExplanation
	//https://stackoverflow.com/questions/42554960/get-xyz-angles-between-vectors

	a.Normalize();
	b.Normalize();

	return acos(a.x * b.x + a.y * b.y);
};

void Player::Update(HeightMap* heightMap)
{
	CalcHeight(heightMap);

	//Rotate camera by cursor movement 
	sceneCamera->Rotate(Event::ReadRawDelta().x * mouseCurrentSensitivity, Event::ReadRawDelta().y * mouseCurrentSensitivity);

	//Get players position last frame and cameras current look-direction
	Vector3 lookDirection = sceneCamera->GetDirection();

	//MOVEMENT DIRECTION
	Vector3 moveDirection;

	if (Event::KeyIsPressed('W'))
		moveDirection += Vector3(0, 0, 1);
	if (Event::KeyIsPressed('S'))
		moveDirection += Vector3(0, 0, -1);
	if (Event::KeyIsPressed('A'))
		moveDirection += Vector3(-1, 0, 0);
	if (Event::KeyIsPressed('D'))
		moveDirection += Vector3(1, 0, 0);

	moveDirection.Normalize();

	//SPRINTING
	if (Event::KeyIsPressed(VK_SHIFT))
	{
		stats.currentSpeed += 5.0f * Time::GetDelta();
		if (stats.currentSpeed > stats.sprintSpeed)
			stats.currentSpeed = stats.sprintSpeed;

		currentCameraDistance += Time::GetDelta() * 10.0f;
		if (currentCameraDistance > maxCameraDistance)
			currentCameraDistance = maxCameraDistance;
	}

	else
	{
		stats.currentSpeed -= 12.0f * Time::GetDelta();
		if (stats.currentSpeed < stats.movementSpeed)
			stats.currentSpeed = stats.movementSpeed;

		currentCameraDistance -= Time::GetDelta() * 7.0f;
		if (currentCameraDistance < defaultCameraDistance)
			currentCameraDistance = defaultCameraDistance;
	}

	//Calculate the radians between the cameras yAxis direction and {0, 0, 1}-Vector.
	//Aligns the keyboardinputs by the camera direction afterwards via the radian.
	if (!Event::LeftIsClicked())
	{
		movementYRadiant = Get2DAngle({ lookDirection.x, lookDirection.z }, { 0, 1 });
		if (lookDirection.x < 0)
			movementYRadiant *= -1;

		Vector2 camera2D = { lookDirection.y, lookDirection.z };
		movementXRadiant = acos(lookDirection.Dot(Vector3(0, 1, 0)) / lookDirection.Length());
		/*camera2D = { lookDirection.y, lookDirection.x };
		float tempRad = acos(camera2D.Dot(Vector2(1, 0)) / camera2D.Length());
		if (tempRad < movementXRadiant)
			movementXRadiant = tempRad;*/
	}

	Matrix movementOfsetMatrix = Matrix::CreateRotationY(movementYRadiant);
	moveDirection = Vector3::Transform(moveDirection, movementOfsetMatrix);
	Vector3 camSocketUpdate = Vector3::Transform(cameraLocationSocket, movementOfsetMatrix);

	//Only update what direction the player is facing when keyboardinput is detected by the moveDirection vector
	if (moveDirection.Length() > 0 || moveDirection.Length() < 0 || Event::RightIsClicked())
		rotation = { 0, movementYRadiant, 0 };

	//Updates the player and cameras positions
	Vector3 newPlayerPos = position + (moveDirection * stats.currentSpeed * Time::GetDelta());

	// JUMPING 
	if (!jumping)
	{
		if (Event::KeyIsPressed(VK_SPACE))
		{
			jumping = true;
			preJumpGroundLevel = heightMapGroundLevel;
		}
	}

	if (jumping)
	{
		airTime += Time::GetDelta() * 5.0f;
		newPlayerPos.y = -powf(airTime, 2) + jumpHeight * airTime + preJumpGroundLevel;
	}

	// RESET TO THE "GROUND"
	if (newPlayerPos.y < heightMapGroundLevel)
	{
		airTime = 0;
		pressed = false;
		jumping = false;
		newPlayerPos = Vector3(newPlayerPos.x, heightMapGroundLevel, newPlayerPos.z);
	}

	position = newPlayerPos + Vector3(0, 3.5f, 0);

	Vector3 newCameraPos = position + (lookDirection * -currentCameraDistance) + Vector3(0.0f, 2.0f, 0.0f);

	if(Event::RightIsClicked())
	{
		newCameraPos = position + camSocketUpdate;
		mouseCurrentSensitivity = mouseAimSensitivity;
		if (Event::LeftIsClicked())
		{
			for (int i = 0; i < arrows.size(); i++)
			{
				arrows.at(i)->Shoot(lookDirection, newPlayerPos + camSocketUpdate, { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
				std::cout << "SHOOT" << std::endl;
			}
		}
	}
	else
	{
		mouseCurrentSensitivity = mouseDefaultSensitivity;
	}
	for (int i = 0; i < arrows.size(); i++)
	{
		arrows.at(i)->Update();
	}

	sceneCamera->MoveTowards(newCameraPos);

	ray->origin = position + Vector3(0, 3, 0);
	Vector3 rayDirection = sceneCamera->GetDirection() + Vector3(0, 0.15, 0);
	rayDirection.Normalize();
	ray->direction = rayDirection;

	Model::Update();

	bounds->Update();
	ray->Update();
	frustum->Update();
}

void Player::Save(const std::string file)
{
	auto filePath = FileSystem::ProjectDirectory::path + "\\SaveData\\" + file + ".pl";

	std::ofstream writer;

	writer.open(filePath, std::ios::beg);
	if (!writer.is_open())
	{
		Print("FAILED TO WRITE PLAYER FILE");
		return;
	}

	const std::string space = " ";

	//STATS
	writer << stats.barbariansKilled << space;
	writer << stats.movementSpeed << space;
	writer << stats.sprintSpeed << space;
	writer << stats.maxHealthPoints << space;
	writer << stats.healthPoints << space;
	writer << stats.level << space;

	//INVENTORY
	writer << inventory.items.size() << space;
	for (auto& [resource, num] : inventory.items)
	{
		writer << (UINT)resource << space;
		writer << num << space;
	}

	writer.close();
	Print("SUCCEEDED SAVING PLAYER FILE");
}

void Player::Load(const std::string file)
{
	auto filePath = FileSystem::ProjectDirectory::path + "\\SaveData\\" + file + ".pl";

	std::ifstream reader;

	reader.open(filePath, std::ios::beg);
	if (!reader.is_open())
	{
		Print("FAILED TO READ PLAYER FILE");
		return;
	}

	//STATS
	reader >> stats.barbariansKilled;
	reader >> stats.movementSpeed;
	reader >> stats.sprintSpeed;
	reader >> stats.maxHealthPoints;
	reader >> stats.healthPoints;
	reader >> stats.level;
	stats.currentSpeed = stats.movementSpeed;

	//INVENTORY
	UINT numItems;
	reader >> numItems;
	for (UINT i = 0; i < numItems; ++i)
	{
		UINT resourceID, num;
		reader >> resourceID;
		reader >> num;

		inventory.items[(RESOURCE)resourceID] = num;
	}

	reader.close();
	Print("SUCCEDED LOADING PLAYER FILE");
}