#ifdef _DEBUG
	#include "DebugApplication.h"
//#else
	#include "Application.h"
#endif

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	//DebugApplication app(hInstance);
//#else
	Application app(hInstance);
#endif
	app.Run();

	return 0;
}