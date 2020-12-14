#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
	Texture();
	~Texture() = default;

	void Create();
	void Upload(unsigned int width, unsigned int height, unsigned char* data);
	void Destroy();

	unsigned int GetID() const { return ID; }

	std::string GetName() const { return name; }
	void SetName(const std::string& n) { name = n; }

private:
	unsigned int ID;

	std::string name;
};

#endif