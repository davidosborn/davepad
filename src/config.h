#ifndef    codepad_config_h
#   define codepad_config_h

#	define PACKAGE   codepad
#	define NAME      CodePad
#	define SUBTITLE  A minimalist workspace for programmers
#	define AUTHOR    David Osborn
#	define URL       
#	define BUGREPORT davidcosborn@gmail.com
#	define COPYRIGHT Copyright (c) 2012 David Osborn

#	define TITLE NAME: SUBTITLE

#	define VERSION_MAJOR 1
#	define VERSION_MINOR 0
#	define VERSION_PATCH 0

#	if VERSION_PATCH == 0
#		define VERSION VERSION_MAJOR.VERSION_MINOR
#	else
#		define VERSION VERSION_MAJOR.VERSION_MINOR.VERSION_PATCH
#	endif

#	define STRINGIZE2(x) #x
#	define STRINGIZE(x) STRINGIZE2(x)

	// GCC doesn't support these keywords yet
#	define final
#	define override

	// Win32 configuration
#	define UNICODE
#	define _UNICODE
#	define WINVER 0x501
#	define _WIN32_IE 0x300

#endif
