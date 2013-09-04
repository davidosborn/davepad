#include <sstream> // ostringstream
#include <stdexcept> // runtime_error

#include <windows.h>
#include <richedit.h>

#include "CodeEditor.hpp"
#include "global.hpp" // GetProgramInstance

/*-------------------------+
| constructor & destructor |
+-------------------------*/

CodeEditor::CodeEditor(Window &parent) :
	Window(MakeWindow(parent.GetHandle()), parent)
{
	Subclass(&CodeEditor::WindowProc);

	SetFont();
	OptimizeGutterSize();
}

CodeEditor::~CodeEditor()
{
	if (font)
	{
		DeleteObject(font);
		font = nullptr;
	}
}

HWND CodeEditor::MakeWindow(HWND parent)
{
	LoadLibrary(TEXT("riched20.dll"));

	// create window
	HWND wnd = CreateWindowEx(
		WS_EX_TRANSPARENT,
		RICHEDIT_CLASS,
		0,
		WS_CHILD |
		WS_HSCROLL |
		WS_VISIBLE |
		WS_VSCROLL |
		ES_AUTOHSCROLL |
		ES_AUTOVSCROLL |
		ES_MULTILINE |
		ES_NOHIDESEL |
		ES_SAVESEL |
		ES_SELECTIONBAR |
		ES_WANTRETURN,
		0, 0, 0, 0, // position and size are initialized in WM_SIZE
		parent,
		0,
		GetProgramInstance(),
		0);
	if (!wnd)
		throw std::runtime_error("failed to create richedit control");

	return wnd;
}

/*-----------------+
| message handling |
+-----------------*/

LRESULT CodeEditor::WindowProc(const NextWindowProc &nextWindowProc, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_ERASEBKGND:
		{
			RECT clientRect;
			GetClientRect(GetHandle(), &clientRect);
			RECT
				formatRect = clientRect,
				gutterRect = clientRect;
			formatRect.left += gutterSize;
			gutterRect.right = gutterSize;

			HDC dc = reinterpret_cast<HDC>(wparam);
			FillRect(dc, &formatRect, reinterpret_cast<HBRUSH>(COLOR_WINDOW));
			FillRect(dc, &gutterRect, reinterpret_cast<HBRUSH>(COLOR_APPWORKSPACE));
		}
		return 1;

		case WM_PAINT:
		{
			RECT updateRect;
			if (GetUpdateRect(GetHandle(), &updateRect, false))
			{
				if (updateRect.left >= gutterSize)
				{
					updateRect.left = 0;
					InvalidateRect(GetHandle(), &updateRect, false);
				}

				nextWindowProc(msg, wparam, lparam);

				RECT clientRect;
				GetClientRect(GetHandle(), &clientRect);
				RECT
					formatRect = clientRect,
					gutterRect = clientRect;
				formatRect.left += gutterSize;
				gutterRect.right = gutterSize;

				HDC dc = GetDC(GetHandle());
				SelectObject(dc, font);
				SetBkMode(dc, TRANSPARENT);

				RECT gutterClipRect = gutterRect;
				MapWindowPoints(GetHandle(), GetParent()->GetHandle(), reinterpret_cast<LPPOINT>(&gutterClipRect), 2);
				IntersectClipRect(dc, gutterClipRect.left, gutterClipRect.top, gutterClipRect.right, gutterClipRect.bottom);

				int scrollPos = [this]
				{
					POINTL p;
					SendMessage(GetHandle(), EM_POSFROMCHAR, reinterpret_cast<WPARAM>(&p), 0);
					return -p.y;
				}();

				RECT gutterTextRect = gutterRect;
				gutterTextRect.top -= (scrollPos % fontLeading);

				unsigned firstLine = scrollPos / fontLeading;
				unsigned numLines = (gutterTextRect.bottom - gutterTextRect.top) / fontLeading + 1;

				std::ostringstream ss;
				for (unsigned i = 1; i < numLines + 1; ++i)
					ss << i + firstLine << '\n';

				DrawTextA(
					dc,
					ss.str().c_str(),
					-1,
					&gutterTextRect,
					0);

				ReleaseDC(GetHandle(), dc);
			}
		}
		return 0;

		case WM_HSCROLL:
		{
			switch (LOWORD(wparam))
			{
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
				{
					SCROLLINFO si =
					{
						sizeof si,
						SIF_TRACKPOS
					};
					GetScrollInfo(GetHandle(), SB_HORZ, &si);
					si.fMask = SIF_POS;
					si.nPos = si.nTrackPos;
					SetScrollInfo(GetHandle(), SB_HORZ, &si, false);
				}
				break;
			}
		}
		break;

		case WM_VSCROLL:
		{
			switch (LOWORD(wparam))
			{
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
				{
					SCROLLINFO si =
					{
						sizeof si,
						SIF_TRACKPOS
					};
					GetScrollInfo(GetHandle(), SB_VERT, &si);
					si.fMask = SIF_POS;
					si.nPos = si.nTrackPos;
					SetScrollInfo(GetHandle(), SB_VERT, &si, false);
				}
				break;
			}
		}
		break;
	}
	return nextWindowProc(msg, wparam, lparam);
}

/*---------------+
| font functions |
+---------------*/

HFONT CodeEditor::GetFont() const
{
	return font;
}

unsigned CodeEditor::GetFontLeading() const
{
	return fontLeading;
}

void CodeEditor::SetFont()
{
	SetFont("Courier New", 11);
}
void CodeEditor::SetFont(const std::string &name, unsigned size)
{
	// calculate font height in pixels
	HDC dc = GetDC(0);
	int height = size * GetDeviceCaps(dc, LOGPIXELSY) / 72;
	ReleaseDC(0, dc);

	// create font
	HFONT font = CreateFontA(
		height,    // height
		0,         // width
		0,         // escapement
		0,         // orientation
		FW_NORMAL, // weight
		false,     // italic
		false,     // underline
		false,     // strike-out
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FIXED_PITCH | FF_ROMAN,
		name.c_str());
	if (!font)
		throw std::runtime_error("failed to create font object");

	try
	{
		SetFont(font);
	}
	catch (...)
	{
		DeleteObject(font);
		throw;
	}
}
void CodeEditor::SetFont(HFONT font)
{
	SendMessage(GetHandle(), WM_SETFONT, reinterpret_cast<WPARAM>(font), 0);

	// get font-leading measurement
	HDC dc = GetDC(GetHandle());
	SelectObject(dc, font);
	TEXTMETRIC tm;
	GetTextMetrics(dc, &tm);
	ReleaseDC(GetHandle(), dc);
	fontLeading = tm.tmHeight;

	this->font = font;
}

/*-----------------+
| gutter functions |
+-----------------*/

unsigned CodeEditor::GetGutterSize() const
{
	return gutterSize;
}

void CodeEditor::OptimizeGutterSize()
{
	gutterSize = 40;

	// update formatting rectangle
	RECT formatRect;
	GetClientRect(GetHandle(), &formatRect);
	formatRect.left = gutterSize;
	SendMessage(GetHandle(), EM_SETRECT, 0, reinterpret_cast<WPARAM>(&formatRect));
}

/*----------------+
| caret functions |
+----------------*/

unsigned CodeEditor::GetCaretLineIndex() const
{
	DWORD nextCharIndex;
	SendMessage(GetHandle(), EM_GETSEL, 0, reinterpret_cast<LPARAM>(&nextCharIndex));
	return SendMessage(GetHandle(), EM_EXLINEFROMCHAR, 0, nextCharIndex);
}
