#include "FrameBuffer.h"

#include <glad/glad.h>

void FrameBuffer::Upload(int bufferWidth, int bufferHeight)
{
	width = bufferWidth;
	height = bufferHeight;

	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &renderTexture);
	glGenRenderbuffers(1, &RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf_s("ERROR: The frame buffer is incomplete.\n");
	}
}

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &renderTexture);
	glDeleteRenderbuffers(1, &RBO);
}
