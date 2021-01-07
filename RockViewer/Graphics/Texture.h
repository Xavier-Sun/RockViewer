#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "Graphics.h"

class Texture : public Graphics
{
public:
	void Upload(unsigned int width, unsigned int height, unsigned char* data);

	unsigned int GetID() const { return ID; }

	Texture();
	~Texture();

private:
	unsigned int ID = 0;
};

#endif