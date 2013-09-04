#ifndef    davepad_MainWindow_hpp
#   define davepad_MainWindow_hpp

#	include "Window.hpp"

class MainWindow : public Window
{
	/*------------+
	| constructor |
	+------------*/

	public:
	MainWindow();

	private:
	static HWND MakeWindow();
	static LPTSTR GetClass();

	/*-----------------+
	| message handling |
	+-----------------*/

	private:
	WindowProcType WindowProc;
};

#endif
