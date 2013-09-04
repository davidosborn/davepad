#ifndef    davepad_AppWindow_hpp
#   define davepad_AppWindow_hpp

#	include <memory> // unique_ptr

#	include "MainWindow.hpp"

class CodeEditor;
class StatusBar;

class AppWindow : public MainWindow
{
	/*-------------------------+
	| constructor & destructor |
	+-------------------------*/

	public:
	AppWindow();
	~AppWindow();

	/*-----------------+
	| message handling |
	+-----------------*/

	private:
	WindowProcType WindowProc;

	/*-------------+
	| data members |
	+-------------*/

	private:
	std::unique_ptr<CodeEditor> codeEditor;
	std::unique_ptr<StatusBar> statusBar;
};

#endif
