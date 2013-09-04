#include <stdexcept> // exception

#include <windows.h>

#include "AppWindow.hpp"
#include "global.hpp" // RunMainLoop

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		AppWindow appWindow;
		RunMainLoop();
	}
	catch (const std::exception &e)
	{
		MessageBoxA(0, e.what(), 0, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return 1;
	}
	return 0;
}
