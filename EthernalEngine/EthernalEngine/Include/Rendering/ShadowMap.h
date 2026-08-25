#pragma once
#include <glad/glad.h>

namespace EthernalEngine
{
	class ShadowMap
	{
	public:
		void Create(unsigned int width, unsigned int height);
		void BindForWriting();
		void BindTexture(GLuint textureUnit);
		void Unbind(unsigned int windowWidth, unsigned int windowHeight);

		GLuint GetDepthMapTexture() const { return depthMap; }

	private:
		GLuint depthMapFBO;
		GLuint depthMap;
		GLint previousFramebuffer = 0;
		unsigned int width, height;
	};
}
