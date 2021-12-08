//#ifdef _DEBUG
	#include "DebugApplication.h"
	#include "Application.h"
//#else
//	#include "Application.h"
//#endif

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	int exitCode = 0;

	{
//#ifdef _DEBUG
	DebugApplication app(hInstance);
//#else
	//Application app(hInstance);
//#endif

	exitCode = app.Run();
	}

//#ifdef _DEBUG
	if (exitCode != 0)
	{
		Application gameApp(hInstance);
		gameApp.Run();
	}
//#endif

	return exitCode;
}