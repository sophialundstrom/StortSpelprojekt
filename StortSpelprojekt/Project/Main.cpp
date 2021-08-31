#include "Application.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Application app(1600u, 900u, L"3D Project", hInstance);
	app.Run();

	return 0;
}