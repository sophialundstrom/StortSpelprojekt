#include "Player.h"
#include "Building.h"


Player::Player(const std::string file, Camera* camera, const UINT& maxArrows)
	:AnimatedModel("MainCharacter", "Player"), sceneCamera(camera)
{
	isRightPressed = false;
	isLeftPressed = false;

	arrowHandler.SetPullbackFactor(1.f);

	bounds = std::make_shared<BoundingBox>();

	bounds->SetScale(0.8f, 2.5f, 0.8f);
	bounds->SetPosition(0, 3, 0);

	frustum = std::make_shared<FrustumCollider>(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 10.0f);
	frustum->SetPosition(0, 3, 0);

	Load(file);

	PlayAnimation("Idle");

	sceneCamera->updatecamRay(position + Vector3(0.0f, 5.0f, 0.0f), 1000);

	currentBiome = BIOME::DEFAULT;
	previousBiome = currentBiome;

	//BOW
	bow = std::make_shared<AnimatedModel>("AnimatedBow", "Bow");
	AMR->Bind(bow);
	minCameraDistance = 0.5f;

}

void Player::CalcHeight(HeightMap* heightMap)
{
	int lowX = (int)std::floor(position.x);
	int highX = (int)std::ceil(position.x);
	float Xdecimal = position.x - lowX;

	int lowZ = (int)std::floor(position.z);
	int highZ = (int)std::ceil(position.z);
	float Zdecimal = position.z - lowZ;

	float H1 = heightMap->data.at(Vector2((float)lowX, (float)lowZ)) * (1 - Xdecimal) * (1 - Zdecimal);
	float H2 = heightMap->data.at(Vector2((float)highX, (float)highZ)) * Xdecimal * Zdecimal;
	float H3 = heightMap->data.at(Vector2((float)lowX, (float)highZ)) * (1 - Xdecimal) * Zdecimal;
	float H4 = heightMap->data.at(Vector2((float)highX, (float)lowZ)) * Xdecimal * (1 - Zdecimal);

	heightMapGroundLevel = position.y = (H1 + H2 + H3 + H4);
}

float Player::CalcHeightForCamera(HeightMap* heightMap)
{
	const int lowX = (int)std::floor(sceneCamera->GetPosition().x);
	const int highX = (int)std::ceil(sceneCamera->GetPosition().x);
	const float Xdecimal = sceneCamera->GetPosition().x - lowX;

	const int lowZ = (int)std::floor(sceneCamera->GetPosition().z);
	const int highZ = (int)std::ceil(sceneCamera->GetPosition().z);
	const float Zdecimal = sceneCamera->GetPosition().z - lowZ;

	const float H1 = heightMap->data.at(Vector2((float)lowX, (float)lowZ)) * (1 - Xdecimal) * (1 - Zdecimal);
	const float H2 = heightMap->data.at(Vector2((float)highX, (float)highZ)) * Xdecimal * Zdecimal;
	const float H3 = heightMap->data.at(Vector2((float)lowX, (float)highZ)) * (1 - Xdecimal) * Zdecimal;
	const float H4 = heightMap->data.at(Vector2((float)highX, (float)lowZ)) * Xdecimal * (1 - Zdecimal);

	float newY = H1 + H2 + H3 + H4;

	return newY;
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
	lastPosition = position;

	std::cout << "Player X: " << position.x << "        " << "Player Y: " << position.y << "        "  << "Player Z: " << position.z << "\n";

	CalcHeight(heightMap);

	if (!hasCollided)
		currentGroundLevel = heightMapGroundLevel;

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
		// IF PLAYER SPRINTS AND JUMPS THE SOUND WILL STOP UNTIL SHIFT IS PRESSED AGAIN...
		if (!isSprinting)
		{
			
			Audio::StartEffect("Running.wav");
			Audio::SetVolume("Running.wav", Audio::effectsVolume * 0.25f);
			isSprinting = true;
		}

		stats.currentSpeed += 70.0f * Time::GetDelta();
		if (stats.currentSpeed > stats.sprintSpeed)
			stats.currentSpeed = stats.sprintSpeed;

		currentCameraDistance += Time::GetDelta() * 20.0f;
		if (currentCameraDistance > maxCameraDistance)
			currentCameraDistance = maxCameraDistance;

		if (currentCameraDistance < minCameraDistance)
		{
			PrintS("LESS");
			currentCameraDistance = minCameraDistance;
		}
	}
	else
	{
		isSprinting = false;
		Audio::StopEffect("Running.wav");
		stats.currentSpeed -= 12.0f * Time::GetDelta();
		if (stats.currentSpeed < stats.movementSpeed)
			stats.currentSpeed = stats.movementSpeed;

		currentCameraDistance -= Time::GetDelta() * 10.0f;
		if (currentCameraDistance < defaultCameraDistance)
			currentCameraDistance = defaultCameraDistance;

		if (currentCameraDistance < minCameraDistance)
		{
			PrintS("LESS");
			currentCameraDistance = minCameraDistance;
		}
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
	}

	Matrix movementOfsetMatrix = Matrix::CreateRotationY(movementYRadiant);
	moveDirection = Vector3::Transform(moveDirection, movementOfsetMatrix);
	Vector3 camSocketUpdate = Vector3::Transform(cameraLocationSocket, movementOfsetMatrix);

	//Only update what direction the player is facing when keyboardinput is detected by the moveDirection vector
	if (moveDirection.Length() > 0 || moveDirection.Length() < 0 || Event::RightIsClicked())
		rotation =  Quaternion::CreateFromYawPitchRoll(movementYRadiant, 0, 0);

	//Updates the player and cameras positions
	Vector3 newPlayerPos = position + (moveDirection * stats.currentSpeed * Time::GetDelta());

	// JUMPING 
	if (!jumping)
	{
		if (Event::KeyIsPressed(VK_SPACE))
		{
			jumping = true;
			Audio::StartEffect("Jump.wav");
			preJumpGroundLevel = currentGroundLevel; 
			//PlayAnimation("Jump", false, 0.5f);
		}
	}

	if (jumping)
	{
		airTime += Time::GetDelta() * 8.0f;

		/*if (airTime >= 1.5f)
			PlayAnimation("Falling", true);*/

		/*else
			std::cout << "Startup" << std::endl;*/

		newPlayerPos.y = -powf(airTime, 2) + jumpHeight * airTime + preJumpGroundLevel;
	}

	// RESET TO THE "GROUND"
	if (newPlayerPos.y < currentGroundLevel)
	{
		airTime = 0;
		pressed = false;
		jumping = false;
		newPlayerPos = Vector3(newPlayerPos.x, currentGroundLevel, newPlayerPos.z);
	}

	if (closestColliderToCam < currentCameraDistance && closestColliderToCam > minCameraDistance)

	{
		currentCameraDistance = closestColliderToCam;
		PrintS("FORCE");
	}

	position = newPlayerPos;

	Vector3 newCameraPos;

	bool approvedCam = false;

	CalcHeightForCamera(heightMap);

	newCameraPos = position + (lookDirection * -currentCameraDistance) + Vector3(0.0f, 5.0f, 0.0f);

	if (Event::KeyIsPressed('8'))
	{
		PrintNumber(-currentCameraDistance, "CURR CAM DIST: ");
	}

	float newY = CalcHeightForCamera(heightMap);
	
	if (newY > newCameraPos.y)
	{
		newCameraPos.y = newY;
	}
	
	static float lastClick = 0;
	static float lastEat = 0;

	if (Event::KeyIsPressed('R') && Time::Get() - lastEat > 1.0f)
	{
		//if (stats.healthPoints < 10 && inventory.NumOf(Item::Type::Food) > 0)
		//{
			stats.IncreaseHealthPoints();
		//	inventory.RemoveItem(Item::Type::Food, 1);
		//}
		lastEat = Time::Get();
	}

	if (Event::KeyIsPressed('K'))
	{
		inventory.AddItem(Item::Type::Stick, 10);
		Print("Sticks: ");
		Print(inventory.NumOf(Item::Type::Stick));
	}

	if (Event::KeyIsPressed('L'))
	{
		inventory.AddItem(Item::Type::Stone, 10);
		Print("Stones: ");
		Print(inventory.NumOf(Item::Type::Stone));
	}


	sinceLastShot += Time::GetDelta();
	if (sinceLastShot > shootingAnimationLenght) {

		bool hasMoved = (position == lastPosition) ? false : true;
	}

	if (Event::RightIsClicked())
	{
		if (!isAiming)
		{
			Audio::StartEffect("Bow.wav");
			bow->PlayOverrideAnimation("Draw", "root", true);
			PlayOverrideAnimation("Aim", "Spine1", true);
			isAiming = true;
		}

		newCameraPos = position + camSocketUpdate;
		mouseCurrentSensitivity = mouseAimSensitivity;
		sceneCamera->SetSpeedMultiplier(5.0f);
		sceneCamera->MoveTowards(newCameraPos);

		if (Time::Get() - lastClick > 1.0f)
		{
			if (Event::LeftIsClicked() && numArrows > 0)
			{
				lookDirection = lookDirection.Transform(lookDirection, Matrix::CreateFromYawPitchRoll(-0.02f, 0.02f, 0.0f));

				arrowHandler.AddArrow(lookDirection, bow->GetPosition(), { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });

				Audio::StartEffect("Fire.wav");

				int currentIndex = 0;
				numArrows--;
				sinceLastShot = 0.f;
				lastClick = Time::Get();

				PlayOverrideAnimation("Reload", "Spine1", true, true);
			}
		}
	}
	
	else
	{
		if (isAiming)
		{
			isAiming = false;
			Audio::StopEffect("Bow.wav");
			PlayOverrideAnimation("Stop", "Spine1", false);
		}

		sceneCamera->SetSpeedMultiplier(1.0f);
		mouseCurrentSensitivity = mouseDefaultSensitivity;
		sceneCamera->MoveTowards(newCameraPos);
	}
		
	arrowHandler.Update();

	if (moveDirection.Length() == 0)
		PlayAnimation("Idle");
	else if (isSprinting)
		PlayAnimation("Run");
	else
		PlayAnimation("Walk");

	if (isAiming)
		AnimatedModel::Update(sceneCamera, "Spine3");
	else
		AnimatedModel::Update();

	sceneCamera->updatecamRay(position + Vector3(0.0f, 5.0f, 0.0f), 1000);

	bounds->Update();
	frustum->Update();

	//BOW UPDATE
	auto& socket = skeleton.transforms[skeleton.GetJointID("LWrist")];

	auto bowT = Matrix::CreateScale(1.5f) * Matrix::CreateFromYawPitchRoll(PI_DIV2, PI, 0) * Matrix::CreateTranslation(0.5f, -0.05f, 0.0f);

	Matrix bowTransform = bowT * socket * matrix;

	Vector3 position, scale;
	Quaternion rotation;
	bowTransform.Decompose(scale, rotation, position);

	rotation.Normalize();

	bow->SetPosition(position);
	bow->SetRotation(rotation);
	bow->SetScale(scale);

	bow->Update();
}

void Player::TakeDamage(int x)
{
	if (stats.healthPoints - 1 == 0 /*|| stats.healthPoints < 0*/)
	{
		stats.healthPoints--;
		gameOver = true;
		return; // Return here because hp will be -1. This leads to a hp image not being found which in turn leads to a crash during Draw().
	}

	//SoundEffect::AddAudio(L"Audio/Damage.wav", 2);
	//SoundEffect::SetVolume(0.5, 2);
	//SoundEffect::StartAudio(2);

	int totalDamage = x - stats.resist;
	if (totalDamage <= 1)
		stats.healthPoints--;
	else
		stats.healthPoints -= totalDamage;
	
}

void Player::SwitchBiomeMusic()
{
	switch (this->currentBiome)
	{
	case BIOME::DESERT:

		Audio::StartMusic("SoundDesert.wav");
		break;
	case BIOME::MOUNTAIN:

		Audio::StartMusic("SoundMountain.wav");
		break;
	case BIOME::OCEAN:
		Audio::StartMusic("SoundOcean.wav");
		break;
	case BIOME::DEFAULT:
		Audio::StartMusic("SoundForest.wav");
		break;
	}
}

void Player::HandleCollidedObjects(const std::vector<std::shared_ptr<Collider>> colliders)
{
	if (colliders.empty())
	{
		hasCollided = false;
		return;
	}

	hasCollided = true;

	auto lastPos = position;

	float highestPoint = lastPos.y;

	while (true)
	{
		Vector3 force;
		UINT numNotCollided = 0;

		for (auto& collider : colliders)
		{
			auto box = std::dynamic_pointer_cast<BoundingBox>(collider);
			if (box)
			{
				if (!Collision::Intersection(box, this->bounds))
					numNotCollided++;

				else
				{
					bool stuck = true;
					Plane planes[6];
					box->GetPlanes(planes);
					for (auto plane : planes)
						if (this->bounds->GetBounds().Intersects(plane) == DirectX::PlaneIntersectionType::INTERSECTING)
						{
							stuck = false;
							force += plane.Normal();
							//std::cout << "NORMAL: " << plane.Normal().x << " " << plane.Normal().y << " " << plane.Normal().z << std::endl;
						}

					if (stuck)
						force += position - box->GetBounds().Center;
				}

				continue;
			}

			auto sphere = std::dynamic_pointer_cast<BoundingSphere>(collider);
			if (sphere)
			{
				auto& bounds = sphere->GetBounds();

				if (!Collision::Intersection(sphere, this->bounds))
					numNotCollided++;
				else
				{
					const auto forceDirection = position - bounds.Center;

					if (forceDirection.Dot({ 0, 1, 0 }) > 0)
						force += forceDirection;
					else
						force += { forceDirection.x, -forceDirection.y, forceDirection.z} ;
				}

				continue;
			}	
		}

		if (numNotCollided == (UINT)colliders.size())
		{
			Transform::UpdateMatrix();
			bounds->Update();
			sceneCamera->updatecamRay(position, 1000);
			return;
		}

		force.Normalize();

		if (force.Dot({ 0, 1, 0 }) < 0)
			force.y *= -1;

		position += force * Time::GetDelta();

		Transform::UpdateMatrix();
		bounds->Update();
	}
}

void Player::HandleUpgrades(std::shared_ptr<Building> building)
{
	std::string buildingName = building->GetBuildingName();
	int state = building->GetCurrentState();
	if (buildingName == "FarmHouse")
	{
		if (state == 1)
		{
			stats.resist = 1;
			stats.HPGain = 2;
		}
		if (state == 2)
		{
			stats.resist = 2;
			stats.HPGain = 3;
		}
	}
	if (buildingName == "ArcherTent")
	{
		if (state == 1)
		{
			maxArrows = 20;
			numArrows = 20;
		}
		if (state == 2)
		{
			maxArrows = 30;
			numArrows = 30;
		}
		if (state == 3)
			numArrows = 30;
	}
	if (buildingName == "Blacksmith")
	{
		if (state == 1)
		{
			stats.damage = 2;
		}
		if (state == 2)
		{
			stats.damage = 3;
		}
	}
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

	reader.close();
}