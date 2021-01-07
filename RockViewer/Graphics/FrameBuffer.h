#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "Graphics.h"

class FrameBuffer : public Graphics
{
public:
	void Upload(int bufferWidth = 800, int bufferHeight = 600);

	unsigned int GetFBO() const { return FBO; }
	unsigned int GetRenderTexture() const { return renderTexture; }
	unsigned int GetRBO() const { return RBO; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	FrameBuffer();
	~FrameBuffer();

private:
	unsigned int FBO = 0;
	unsigned int renderTexture = 0;
	unsigned int RBO = 0;
	int width = 0;
	int height = 0;
};

#endif