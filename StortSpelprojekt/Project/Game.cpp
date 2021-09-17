#include "Game.h"
#include "Event.h"

void Game::Update()
{
	

	//scene.GetCamera() Do whatever with this

	//TO DO: FIGURE OUT A NICE MOVEMENT IN EDITOR
	//scene.GetCamera().Rotate(Event::ReadRawData().x, Event::ReadRawData().y);
	//Event::ClearRawDelta();
	//	

	//if (Event::KeyIsPressed('W'))
	//	scene.GetCamera().MoveForward();

	//if (Event::KeyIsPressed('A'))
	//	scene.GetCamera().MoveRight(-1);

	//if (Event::KeyIsPressed('S'))
	//	scene.GetCamera().MoveForward(-1);

	//if (Event::KeyIsPressed('D'))
	//	scene.GetCamera().MoveRight();

	//if (Event::KeyIsPressed(32)) //SPACE
	//	scene.GetCamera().MoveUp();

	//if (Event::KeyIsPressed(16)) //SHIFT
	//	scene.GetCamera().MoveUp(-1);

	//KOLLA UPP ROLL PITCH
	
	Time::GetDelta;


	scene.GetCamera().Rotate(Event::ReadRawData().x * mouseSensitivity, Event::ReadRawData().y * mouseSensitivity);
	Event::ClearRawDelta();

	Vector3 playerPos = scene.Get<Model>("PlayerArrow").get()->GetPosition();
	Vector3 lookDirection = scene.GetCamera().GetDirection();
	float camDistance = 10;

	//Movement system
	Vector3 moveDirection = Vector3(0, 0, 0);

	if (Event::KeyIsPressed(VK_UP))
		moveDirection += Vector3(0, 0, 1);

	if (Event::KeyIsPressed(VK_DOWN))
		moveDirection += Vector3(0, 0, -1);

	if (Event::KeyIsPressed(VK_LEFT))
		moveDirection += Vector3(-1, 0, 0);

	if (Event::KeyIsPressed(VK_RIGHT))
		moveDirection += Vector3(1, 0, 0);
	
	moveDirection.Normalize();
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

	shadowRenderer.Render();

	Graphics::Inst().BeginFrame();

	deferredRenderer.Render();
	
	//RENDER UI PROBABLY

	Graphics::Inst().EndFrame();
}

Game::Game(UINT clientWidth, UINT clientHeight)
	:deferredRenderer(clientWidth, clientHeight), 
	modelRenderer(DEFERRED, true), 
	particleRenderer(DEFERRED)
{
	//LOAD SCENE
	scene.SetCamera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 100.0f, 1.0f, 10.0f, { 0.0f, 2.0f, -10.0f }, { 0.f, 0.f, 1.f }, {0, 1, 0});
	
	scene.SetDirectionalLight(30, 4, 4);

	//Player
	scene.AddModel("PlayerArrow");
	scene.Get<Model>("PlayerArrow")->SetPosition(0, 0, 0);
	scene.Get<Model>("PlayerArrow")->SetRotation({ 0, 0, 0 });
	modelRenderer.Bind(scene.Get<Model>("PlayerArrow"));

	//Plane in Enviroment
	scene.AddModel("Plane");
	scene.Get<Model>("Plane")->SetPosition(0, 0, 0);
	scene.Get<Model>("Plane")->SetRotation(0, 0, 90);
	modelRenderer.Bind(scene.Get<Model>("Plane"));



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