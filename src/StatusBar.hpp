#ifndef    davepad_StatusBar_hpp
#   define davepad_StatusBar_hpp

#	include "Window.hpp"

class StatusBar : public Window
{
	/*------------+
	| constructor |
	+------------*/

	public:
	explicit StatusBar(Window &parent);

	private:
	static HWND MakeWindow(HWND parent);
};

#endif
