#include "Application.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UINT WIDHT = GetSystemMetrics(SM_CXSCREEN);
	UINT HEIGHT = GetSystemMetrics(SM_CYSCREEN);

#ifdef _DEBUG
	WIDHT = 1280;
	HEIGHT = 720;
#endif

	Application app(WIDHT, HEIGHT, L"Stort Spelprojekt", hInstance);
	app.Run();

	return 0;
}