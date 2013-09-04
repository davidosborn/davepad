#include <fstream> // ifstream
#include <memory> // unique_ptr
#include <sstream> // istringstream

#include "File.hpp"

File::File(const std::string &path) :
	path(path)
{
	// determine file size
	std::ifstream is(path.c_str(), std::ios_base::binary | std::ios_base::in);
	unsigned size = is.seekg(0, std::ios_base::end).tellg();
	is.seekg(0, std::ios_base::beg);

	// read file content into string
	const std::unique_ptr<char[]> buffer(new char[size]);
	is.read(buffer.get(), size);
	content.assign(buffer.get(), size);

	// separate content into lines
	std::istringstream ss(content);
	for (std::string line; getline(ss, line).good();)
		lines.push_back(line);
}

std::string &File::GetContent()
{
	return content;
}
const std::string &File::GetContent() const
{
	return content;
}

std::vector<std::string> &File::GetLines()
{
	return lines;
}
const std::vector<std::string> &File::GetLines() const
{
	return lines;
}
