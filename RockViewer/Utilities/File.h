#ifndef FILE_H
#define FILE_H

#include <memory>
#include <fstream>
#include <sstream>

class File
{
public:
	static std::shared_ptr<std::string> ReadFileToString(const char* filePath);
};

#endif