#ifndef    davepad_CodeEditor_hpp
#   define davepad_CodeEditor_hpp

#	include "Window.hpp"

class CodeEditor : public Window
{
	/*-------------------------+
	| constructor & destructor |
	+-------------------------*/

	public:
	explicit CodeEditor(Window &parent);

	~CodeEditor();

	private:
	static HWND MakeWindow(HWND parent);

	/*-----------------+
	| message handling |
	+-----------------*/

	private:
	WindowProcType WindowProc;

	/*---------------+
	| font functions |
	+---------------*/

	private:
	HFONT GetFont() const;
	unsigned GetFontLeading() const;
	void SetFont();
	void SetFont(const std::string &name, unsigned size);
	void SetFont(HFONT);

	/*-----------------+
	| gutter functions |
	+-----------------*/

	private:
	unsigned GetGutterSize() const;
	void OptimizeGutterSize();

	/*----------------+
	| caret functions |
	+----------------*/

	private:
	unsigned GetCaretLineIndex() const;

	/*-------------+
	| data members |
	+-------------*/

	private:
	HFONT    font;
	unsigned fontLeading;
	unsigned gutterSize;
};

#endif
