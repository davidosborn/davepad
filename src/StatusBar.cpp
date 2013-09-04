#include <stdexcept> // runtime_error

#include <windows.h>
#include <commctrl.h> // STATUSCLASSNAME, SBARS_SIZEGRIP

#include "global.hpp" // GetProgramInstance
#include "StatusBar.hpp"

/*------------+
| constructor |
+------------*/

StatusBar::StatusBar(Window &parent) :
	Window(MakeWindow(parent.GetHandle()), parent) {}

HWND StatusBar::MakeWindow(HWND parent)
{
	HWND wnd = CreateWindowEx(
		WS_EX_TRANSPARENT,
		STATUSCLASSNAME,
		0,
		WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
		0, 0, 0, 0,
		parent,
		0,
		GetProgramInstance(),
		0);
	if (!wnd)
		throw std::runtime_error("failed to create status bar");

	return wnd;
}
