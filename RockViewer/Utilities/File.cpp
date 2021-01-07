#include "File.h"

std::shared_ptr<std::string> File::ReadFileToString(const char* filePath)
{
	auto str = std::make_shared<std::string>();
	std::ifstream fileStream;
	std::stringstream stringStream;
	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		fileStream.open(filePath);
		stringStream << fileStream.rdbuf();
		fileStream.close();
		*str = stringStream.str();
	}
	catch (std::ifstream::failure&)
	{
		printf_s("WARNING: Failed to read file.\n");
	}

	return str;
}
