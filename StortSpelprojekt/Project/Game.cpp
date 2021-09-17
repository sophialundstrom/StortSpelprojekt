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

	scene.Update();
}

void Game::Render()
{
	deferredRenderer.SetRenderTargets();

	particleRenderer.Render();

	modelRenderer.Render();

	//terrainRenderer.Render(terrain);

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
	questLog = std::make_unique<QuestLog>(nullptr);

	//LOAD SCENE
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 100.0f, 1.0f, 10.0f, { 0.0f, 2.0f, -10.0f }, { 0.f, 0.f, 1.f }, {0, 1, 0});
	
	scene.SetDirectionalLight(30, 4, 4);

	//Player
	scene.AddModel("PlayerArrow");
	scene.Get<Model>("PlayerArrow")->SetPosition(0, 0, 0);
	scene.Get<Model>("PlayerArrow")->SetRotation({ 0, PI, 0 });
	modelRenderer.Bind(scene.Get<Model>("PlayerArrow"));

	//Plane in Enviroment
	scene.AddModel("Plane");
	scene.Get<Model>("Plane")->SetPosition(0, -1, 0);
	scene.Get<Model>("Plane")->SetRotation(0, 0, 0);
	modelRenderer.Bind(scene.Get<Model>("Plane"));



	//scene.AddModel("staff");
	//modelRenderer.Bind(scene.Get<Model>("staff"));
	//scene.Get<Model>("staff")->SetScale(2.0f);

	//terrain.Draw();

	deferredRenderer.SetRenderTargets();
	(void)Run();
}

Game::~Game()
{
	scene.Clear();
	Resources::Inst().Clear();
	std::cout << "Game Deleted\n";
}

State Game::Run()
{
	Update();
	Render();

	if (Event::KeyIsPressed('M'))
		return State::MENU;

	if (Event::KeyIsPressed(VK_ESCAPE))
		return State::EXIT;

	return State::NO_CHANGE;
}