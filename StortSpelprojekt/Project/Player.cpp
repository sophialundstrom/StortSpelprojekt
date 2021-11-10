#include "Player.h"

Player::Player(const std::string file, Camera* camera, std::shared_ptr<Canvas> ingameCanvas, std::vector<std::shared_ptr<Arrow>> arrows)
	:AnimatedModel("multipleAnimationModel", "Player"), sceneCamera(camera), ingameCanvas(ingameCanvas)
{
	isRightPressed = false;
	isLeftPressed = false;

	this->arrows = arrows;
	bounds = std::make_shared<BoundingBox>();

	bounds->SetScale(0.8f, 2.5f, 0.8f);
	bounds->SetPosition(0, 3, 0);

	frustum = std::make_shared<FrustumCollider>(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 10.0f);
	frustum->SetPosition(0, 3, 0);

	Load(file);
	UpdateHealthUI();

	PlayAnimation("Idle", true, 0.2f);

	sceneCamera->updatecamRay(position + Vector3(0.0f, 5.0f, 0.0f), 1000);
}

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

	if (!hasCollided)
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
		stats.currentSpeed += 50.0f * Time::GetDelta();
		if (stats.currentSpeed > stats.sprintSpeed)
			stats.currentSpeed = stats.sprintSpeed;

		currentCameraDistance += Time::GetDelta() * 20.0f;
		if (currentCameraDistance > maxCameraDistance)
			currentCameraDistance = maxCameraDistance;
	}

	else
	{
		stats.currentSpeed -= 12.0f * Time::GetDelta();
		if (stats.currentSpeed < stats.movementSpeed)
			stats.currentSpeed = stats.movementSpeed;

		currentCameraDistance -= Time::GetDelta() * 10.0f;
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
			preJumpGroundLevel = heightMapGroundLevel; PlayAnimation("Jump", false, 0.5f);
		}
	}

	if (jumping)
	{
		airTime += Time::GetDelta() * 8.0f;

		if (airTime >= 1.5f)
			PlayAnimation("Falling", true);
		else
			std::cout << "Startup" << std::endl;

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

	if (closestColliderToCam < currentCameraDistance)
		currentCameraDistance = closestColliderToCam;

	position = newPlayerPos/* + Vector3(0, 3.5f, 0)*/;

	Vector3 newCameraPos;

	bool approvedCam = false;

	CalcHeightForCamera(heightMap);

	newCameraPos = position + (lookDirection * -currentCameraDistance) + Vector3(0.0f, 5.0f, 0.0f);

	float newY = CalcHeightForCamera(heightMap);
	//std::cout << "HeightMapAtCam: " << CalcHeightForCamera(heightMap) << "			CamHeight: " << sceneCamera->GetPosition().y << "			";
	if (newY > newCameraPos.y)
	{
		//std::cout << "PROBLEMATIC\n";
		newCameraPos.y = newY;
	}
	
	static float lastClick = 0;

	sinceLastShot += Time::GetDelta();
	if (sinceLastShot > shootingAnimationLenght) {

		bool hasMoved = (position == lastPosition) ? false : true;
		if (!hasMoved)
			PlayAnimation("Idle", true, 0.2f); // ADD IDLE ANIMATION
		else if (hasMoved && !jumping)
			PlayAnimation("Walk", true); // ADD WALKING ANIMATION
		//else if (jumping)
			 // ADD IN AIR JUMP ANIMATION
	}

	if(Event::RightIsClicked())
	{
		newCameraPos = position + camSocketUpdate;
		mouseCurrentSensitivity = mouseAimSensitivity;
		sceneCamera->SetPosition(newCameraPos);
		
		if (Time::Get() - lastClick > 0.2f)
		{
			if (Event::LeftIsClicked())
			{
				//PlayAnimation("Take003", false); // ADD SHOOTING ANIMATION
				int currentIndex = 0;
				bool isPlayerShootingArrow = false;
				while(currentIndex < arrows.size() && isPlayerShootingArrow == false)
				{
					isPlayerShootingArrow = arrows.at(currentIndex)->Shoot(lookDirection, newPlayerPos + camSocketUpdate, { PI_DIV2 - movementXRadiant, movementYRadiant, 0 });
					lastClick = Time::Get();
					currentIndex++;
				}
				sinceLastShot = 0.f;
			}
		}
	}

	else
	{
		mouseCurrentSensitivity = mouseDefaultSensitivity;
		sceneCamera->MoveTowards(newCameraPos);
	}
		

	for (int i = 0; i < arrows.size(); i++)
	{
		arrows.at(i)->Update();
	}

	AnimatedModel::Update();
	sceneCamera->updatecamRay(position + Vector3(0.0f, 5.0f, 0.0f), 1000);
	bounds->Update();
	frustum->Update();
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

	Vector3 highestPoint = lastPos;

	while (true)
	{
		Vector3 force;
		UINT numNotCollided = 0;

		for (auto& collider : colliders)
		{
			auto box = std::dynamic_pointer_cast<BoundingBox>(collider);
			if (box)
			{
				Vector3 corners[8];
				box->GetBounds().GetCorners(corners);
				for (auto& corner : corners)
					if (corner.y > highestPoint.y)
						highestPoint = corner;

				if (!Collision::Intersection(box, this->bounds))
					numNotCollided++;
				else
					force += position - box->GetBounds().Center;
			}

			auto sphere = std::dynamic_pointer_cast<BoundingSphere>(collider);
			if (sphere)
			{
				auto& bounds = sphere->GetBounds();

				auto incomingDirection = highestPoint - bounds.Center;

				if (incomingDirection.Length() < bounds.Radius)
				{
					incomingDirection.Normalize();

					auto point = bounds.Center + incomingDirection * bounds.Radius;
					if (highestPoint.y > bounds.Center.y)
						highestPoint = point;
				}

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
			}	
		}

		if (numNotCollided == (UINT)colliders.size())
		{
			position.y = highestPoint.y - 1.0f;
			heightMapGroundLevel = position.y;
			return;
		}

		force.Normalize();

		position += force * Time::GetDelta();

		if (highestPoint.y > lastPos.y)
		{
			position.y = highestPoint.y;
		}

		Transform::UpdateMatrix();
		bounds->Update();
	}
}

bool Player::ProjectileCollided(std::shared_ptr<Arrow>& arrow)
{
	bool collided = false;
	if (Collision::Intersection(this->bounds, arrow->GetCollider()))
	{
		collided = true;
		Print("ARROW HIT PLAYER");
		arrow->DisableArrow();
		if (stats.healthPoints == 0)
		{
			return collided;
		}
		if (stats.healthPoints - 1 == 0)
		{
			stats.healthPoints--;
			std::cout << "GAME OVER" << std::endl;
			UpdateHealthUI();
			gameOver = true;
			return collided;
		}
		stats.healthPoints--;
		UpdateHealthUI();
	}
	return collided;
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

bool Player::CheckArrowHit(std::shared_ptr<Collider> collider)
{
	for (auto& arrow : arrows)
	{
		if (!arrow->IsShot())
			continue;

		bool hit = false;

		auto box = std::dynamic_pointer_cast<BoundingBox>(collider);
		if (box)
			hit = Collision::Intersection(box, arrow->GetCollider());

		auto sphere = std::dynamic_pointer_cast<BoundingSphere>(collider);
		if (sphere)
			hit = Collision::Intersection(sphere, arrow->GetCollider());

		if (hit)
			arrow->DisableArrow();
		
		return hit;
	}

	return false;
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