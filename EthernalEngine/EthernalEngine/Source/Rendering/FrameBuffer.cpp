#include "Rendering/FrameBuffer.h"

#include <iostream>

namespace EthernalEngine
{
	void FrameBuffer::Create(uint32_t width, uint32_t height)
	{
		windowWidth = width;
		windowHeight = height;
		m_width = width;
		m_height = height;

		// Create Framebuffer
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// Create Color Texture
		glGenTextures(1, &m_ColorTexture);
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Bind Color Texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

		// Create Render buffer
		glGenRenderbuffers(1, &m_DepthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
		glRenderbufferStorage(
			GL_RENDERBUFFER,
			GL_DEPTH24_STENCIL8,
			width,
			height
		);

		// Bind Render buffer
		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER,
			m_DepthRBO
		);

		// Check Completeness
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Framebuffer is not complete!\n";
		}

		// Unbind Frame Buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, m_width, m_height);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth, windowHeight);
	}

	void FrameBuffer::Resize(uint32_t newWidth, uint32_t newHeight)
	{
		if (newWidth == 0 || newHeight == 0)
			return;

		if (m_width == newWidth && m_height == newHeight)
			return;

		m_width = newWidth;
		m_height = newHeight;
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}