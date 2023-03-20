#ifndef FILE_TYPES_
#define FILE_TYPES_
#include <iostream>

enum class FileTypes
{
	AUTO,
	TXT,
};


using filename_t = std::string;

constexpr filename_t filetype_to_extension(const FileTypes type)
{
	switch (type)
	{
	case FileTypes::AUTO:
		throw std::runtime_error("No file extension exists for AUTO file type.");
	case FileTypes::TXT:
		return ".txt";
	default:
		throw std::runtime_error("Unknown file type.");
		break;
	}
}

constexpr FileTypes filename_to_filetype(const filename_t& filename)
{
	if (filename.ends_with(".txt")) return FileTypes::TXT;
	throw std::runtime_error("No file type for such a file extension.");
}

#endif // !FILE_TYPES_
