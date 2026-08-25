#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace EthernalEngine
{
	class FrameBuffer
	{
	public:
		void Create(uint32_t width, uint32_t height);
		void Bind();
		void Unbind();
		void Resize(uint32_t newWidth, uint32_t newHeight);
		GLuint GetColorTexture() { return m_ColorTexture; }
		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }

	private:
        uint32_t windowWidth, windowHeight, m_width, m_height;
		GLuint m_FBO;
		GLuint m_ColorTexture;
		GLuint m_DepthTexture;
		GLuint m_DepthRBO;
	};
}