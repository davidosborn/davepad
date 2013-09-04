#include <stdexcept> // runtime_error

#include "../res/resource.h" // IDI_ICON
#include "global.hpp" // GetProgramInstance
#include "MainWindow.hpp"

/*-------------------------+
| constructor & destructor |
+-------------------------*/

MainWindow::MainWindow() :
	Window(MakeWindow())
{
	Subclass(&MainWindow::WindowProc);
}

HWND MainWindow::MakeWindow()
{
	HWND wnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_COMPOSITED,
		GetClass(),
		TEXT(STRINGIZE(TITLE)),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, // left
		CW_USEDEFAULT, // top
		CW_USEDEFAULT, // width
		CW_USEDEFAULT, // height
		0,
		0,
		GetProgramInstance(),
		0);
	if (!wnd)
		throw std::runtime_error("failed to create main window");

	return wnd;
}

LPTSTR MainWindow::GetClass()
{
	static LPTSTR r = []
	{
		WNDCLASSEX wc =
		{
			sizeof wc,
			0,
			DefWindowProc,
			0,
			0,
			GetProgramInstance(),
			LoadIcon(GetProgramInstance(), MAKEINTRESOURCE(IDI_ICON)),
			LoadCursor(GetProgramInstance(), IDC_ARROW),
			0,
			0,
			TEXT(STRINGIZE(PACKAGE)),
			0
		};
		ATOM r = RegisterClassEx(&wc);
		if (!r)
			throw std::runtime_error("failed to register window class");

		return MAKEINTATOM(r);
	}();
	return r;
}

/*-----------------+
| message handling |
+-----------------*/

LRESULT MainWindow::WindowProc(const NextWindowProc &nextWindowProc, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return nextWindowProc(msg, wparam, lparam);
}
