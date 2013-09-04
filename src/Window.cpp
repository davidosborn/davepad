#include <cassert>

#include "Window.hpp"

/*--------------------------+
| constructors & destructor |
+--------------------------*/

Window::Window(HWND handle, Window *parent) :
	handle(handle), parent(parent)
{
	SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	// initialize message handling
	using namespace std::placeholders;
	auto origWindowProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(handle, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WindowProcRouter)));
	windowProc = std::bind(origWindowProc, handle, _1, _2, _3);
}

Window::Window(HWND handle, Window &parent) :
	Window(handle, &parent) {}

Window::~Window() = default;

/*-----------------+
| message handling |
+-----------------*/

void Window::Subclass(LRESULT (Window::*newWindowProc)(const std::function<LRESULT (UINT, WPARAM, LPARAM)> &, UINT, WPARAM, LPARAM))
{
	Subclass(std::function<LRESULT (Window *, const std::function<LRESULT (UINT, WPARAM, LPARAM)> &, UINT, WPARAM, LPARAM)>(newWindowProc));
}
void Window::Subclass(const std::function<LRESULT (Window *, const std::function<LRESULT (UINT, WPARAM, LPARAM)> &, UINT, WPARAM, LPARAM)> &newWindowProc)
{
	using namespace std::placeholders;
	windowProc = std::bind(newWindowProc, this, windowProc, _1, _2, _3);
}

LRESULT Window::WindowProcRouter(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window *instance = reinterpret_cast<Window *>(GetWindowLongPtr(wnd, GWLP_USERDATA));
	assert(instance);
	assert(instance->GetHandle() == wnd);
	return instance->windowProc(msg, wparam, lparam);
}

/*----------+
| observers |
+----------*/

HWND Window::GetHandle() const
{
	return handle;
}

Window *Window::GetParent() const
{
	return parent;
}
