#include <windows.h>
#include <commctrl.h>

#include "init_priority.hpp" // MAX_INIT_PRIORITY

namespace
{
	struct Initializer
	{
		Initializer()
		{
			// initialize common controls with new look-and-feel
			INITCOMMONCONTROLSEX icc =
			{
				sizeof icc,
				ICC_BAR_CLASSES | ICC_STANDARD_CLASSES
			};
			InitCommonControlsEx(&icc);
		}

	} initializer __attribute__((init_priority(MAX_INIT_PRIORITY)));
}

HINSTANCE GetProgramInstance()
{
	static HINSTANCE r = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));
	return r;
}

void RunMainLoop()
{
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
