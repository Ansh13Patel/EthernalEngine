#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

namespace EthernalEngine
{
	class Texture 
	{
	public:
		Texture();
		~Texture();
		bool LoadTextureFromPath(const char* filePath);
		bool LoadTextureFromMemory(unsigned char* data, unsigned int size);
		void Bind() const
		{
			glBindTexture(GL_TEXTURE_2D, m_textureID);
		}
		unsigned int GetTextureID() const { return m_textureID; }

	private:
		unsigned int m_textureID;
		unsigned char* data;
		int width, height, nrChannels;
	};
}