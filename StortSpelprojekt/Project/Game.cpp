#include "Game.h"
#include "Event.h"

void Game::Update()
{
	Time::GetDelta;

	//Rotate camera by cursor movement 
	scene.GetCamera().Rotate(Event::ReadRawDelta().x * mouseSensitivity, Event::ReadRawDelta().y * mouseSensitivity);
	Event::ClearRawDelta();

	//Get players position last frame and cameras current look-direction
	Vector3 playerPos = scene.Get<Model>("PlayerArrow").get()->GetPosition();
	Vector3 lookDirection = scene.GetCamera().GetDirection();

	//Vector that defines the direction the player move
	Vector3 moveDirection = Vector3(0, 0, 0);
	if (Event::KeyIsPressed('W'))
		moveDirection += Vector3(0, 0, 1);
	if (Event::KeyIsPressed('S'))
		moveDirection += Vector3(0, 0, -1);
	if (Event::KeyIsPressed('A'))
		moveDirection += Vector3(-1, 0, 0);
	if (Event::KeyIsPressed('D'))
		moveDirection += Vector3(1, 0, 0);
	moveDirection.Normalize();

	//Calculate the radians between the cameras yAxis direction and {0, 0, 1}-Vector.
	//Aligns the keyboardinputs by the camera direction afterwards via the radian.
	
	if (!Event::RightIsClicked())
	{
		movementOfsetRadiant = get2dAngle({ lookDirection.x, lookDirection.z }, { 0, 1 });
		if (lookDirection.x < 0)
			movementOfsetRadiant *= -1;
	}
	Matrix movementOfsetMatrix = Matrix::CreateRotationY(movementOfsetRadiant);
	moveDirection = Vector3::Transform(moveDirection, movementOfsetMatrix);

	//Only update what direction the player is facing when keyboardinput is detected by the moveDirection vector
	if (moveDirection.Length() > 0 || moveDirection.Length() < 0)
		scene.Get<Model>("PlayerArrow")->SetRotation({ 0, movementOfsetRadiant + PI, 0 });

	//Updates the player and cameras positions
	moveDirection = moveDirection * (playerMoveSpeed * Time::GetDelta());
	Vector3 newPlayerPos = playerPos + moveDirection;
	Vector3 newCameraPos = playerPos + (lookDirection * -camDistance);
	scene.Get<Model>("PlayerArrow")->SetPosition(newPlayerPos);
	scene.GetCamera().SetPosition(newCameraPos);

	QuestLog::Inst().Update();
	scene.Update();
}

void Game::Render()
{
	deferredRenderer.SetRenderTargets();

	particleRenderer.Render();

	modelRenderer.Render();

	terrainRenderer.Render(terrain);

	shadowRenderer.Render();

	Graphics::Inst().BeginFrame();

	deferredRenderer.Render();
	
	//RENDER UI PROBABLY

	Graphics::Inst().EndFrame();
}

Game::Game(UINT clientWidth, UINT clientHeight)
	:deferredRenderer(clientWidth, clientHeight), 
	modelRenderer(DEFERRED, true), 
	particleRenderer(DEFERRED),
	terrainRenderer(DEFERRED), terrain(50.0f)
{
	player = new Player();
	questLog = std::make_unique<QuestLog>(player);

	//LOAD SCENE
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 100.0f, 1.0f, 10.0f, { 0.0f, 2.0f, -10.0f }, { 0.f, 0.f, 1.f }, {0, 1, 0});
	scene.SetDirectionalLight(30, 4, 4);

	//Player
	scene.AddModel("PlayerArrow");
	auto arrow = scene.Get<Model>("PlayerArrow");
	arrow->SetRotation({ 0, PI, 0 });
	arrow->SetPosition(0, 5, 0);
	modelRenderer.Bind(arrow);

	(void)Run();
}

Game::~Game()
{
	delete player;
	scene.Clear();
	Resources::Inst().Clear();
}

State Game::Run()
{
	Update();
	Render();

	if (Event::KeyIsPressed('T'))
		QuestLog::Inst().Activate(0);

	if (Event::KeyIsPressed('U'))
		QuestLog::Inst().Complete(0);

	if (Event::KeyIsPressed('B'))
		player->GameStats().barbariansKilled++;

	if (Event::KeyIsPressed('I'))
		player->Inventory().AddItem(0);

	if (Event::KeyIsPressed('M'))
		return State::MENU;

	if (Event::KeyIsPressed(VK_ESCAPE))
		return State::EXIT;

	return State::NO_CHANGE;
}