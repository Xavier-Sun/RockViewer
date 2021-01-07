#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>

class Graphics
{
public:
	std::string name;

	Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
};

#endif