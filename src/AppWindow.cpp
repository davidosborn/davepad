#include "AppWindow.hpp"
#include "CodeEditor.hpp"
#include "StatusBar.hpp"

/*-------------------------+
| constructor & destructor |
+-------------------------*/

AppWindow::AppWindow()
{
	Subclass(&AppWindow::WindowProc);

	codeEditor.reset(new CodeEditor(*this));
	statusBar.reset(new StatusBar(*this));

	SetForegroundWindow(GetHandle());
	SetFocus(codeEditor->GetHandle());
	ShowWindow(GetHandle(), SW_SHOW);
}

AppWindow::~AppWindow() = default;

/*-----------------+
| message handling |
+-----------------*/

LRESULT AppWindow::WindowProc(const NextWindowProc &nextWindowProc, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_SETFOCUS:
		SetFocus(codeEditor->GetHandle());
		break;

		case WM_SIZE:
		{
			SendMessage(statusBar->GetHandle(), WM_SIZE, 0, 0);

			RECT clientRect, statusBarRect;
			GetClientRect(GetHandle(), &clientRect);
			GetClientRect(statusBar->GetHandle(), &statusBarRect);

			MoveWindow(
				codeEditor->GetHandle(),
				clientRect.left,
				clientRect.top,
				clientRect.right,
				clientRect.bottom - statusBarRect.bottom,
				true);
		}
		break;
	}
	return nextWindowProc(msg, wparam, lparam);
}
