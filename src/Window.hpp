#ifndef    davepad_Window_hpp
#   define davepad_Window_hpp

#	include <functional> // function

#	include <windows.h>

class Window
{
	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	public:
	explicit Window(HWND handle, Window *parent = nullptr);
	Window(HWND handle, Window &parent);

	virtual ~Window();

	/*-----------------+
	| message handling |
	+-----------------*/

	public:
	typedef std::function<LRESULT (UINT, WPARAM, LPARAM)> NextWindowProc;
	typedef LRESULT WindowProcType(const NextWindowProc &, UINT, WPARAM, LPARAM);

	protected:
	template <typename T>
		void Subclass(LRESULT (T::*windowProc)(const NextWindowProc &, UINT, WPARAM, LPARAM))
	{
		Subclass(static_cast<LRESULT (Window::*)(const NextWindowProc &, UINT, WPARAM, LPARAM)>(windowProc));
	}

	private:
	void Subclass(LRESULT (Window::*)(const NextWindowProc &, UINT, WPARAM, LPARAM));
	void Subclass(const std::function<LRESULT (Window *, const NextWindowProc &, UINT, WPARAM, LPARAM)> &);

	private:
	static LRESULT CALLBACK WindowProcRouter(HWND, UINT, WPARAM, LPARAM);

	/*----------+
	| observers |
	+----------*/

	public:
	HWND GetHandle() const;
	Window *GetParent() const;

	/*-------------+
	| data members |
	+-------------*/

	private:
	HWND handle;
	Window *parent;
	NextWindowProc windowProc;
};

#endif
