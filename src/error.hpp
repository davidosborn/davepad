#include <sstream> // ostringstream

std::string GetWin32ErrorString(DWORD errorCode)
{
	char *buffer;
	if (!FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		0,
		errorCode,
		0,
		reinterpret_cast<LPSTR>(buffer),
		0,
		0))
			throw std::runtime_error("failed to format error message");
	std::string s(buffer);
	LocalFree(buffer);
	return s;
}
void ThrowWin32Error(
	const std::string &file,
	unsigned line,
	const std::string &function)
{
	std::ostringstream ss;
	DWORD errorCode = GetLastError();
	//ss << file << ":" << line << ": " << function;// << " failed with error " << errorCode << ": " << GetWin32ErrorString(errorCode);
	ss << errorCode;// << ": " << GetWin32ErrorString(errorCode);
	throw std::runtime_error(ss.str());
}

#define THROW_ERROR \
	ThrowWin32Error(__FILE__, __LINE__, __FUNCTION__)
