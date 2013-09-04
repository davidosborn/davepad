#ifndef    davepad_File_hpp
#   define davepad_File_hpp

#	include <string>
#	include <vector>

class File
{
	public:
	explicit File(const std::string &path);

	public:
	std::string &GetContent();
	const std::string &GetContent() const;
	std::vector<std::string> &GetLines();
	const std::vector<std::string> &GetLines() const;

	private:
	std::string path;
	std::string content;
	std::vector<std::string> lines;
};

#endif
