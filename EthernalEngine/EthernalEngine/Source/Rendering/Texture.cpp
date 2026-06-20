#include "Rendering/Texture.h"

#include <iostream>

namespace EthernalEngine
{
	Texture::Texture()
	{
		data = nullptr;
		width = 0;
		height = 0;
		nrChannels = 0;
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	Texture::~Texture()
	{
		if (m_textureID != 0)
		{
			glDeleteTextures(1, &m_textureID);
		}
	}

	bool Texture::LoadTextureFromPath(const char* filePath)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(filePath, &width, &height, &nrChannels, 0);

		if (!data) {
			std::cout << "Failed to load texture: " << filePath << std::endl;
			return false;
		}

		GLenum format;
		if (nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		return true;
	}

	bool Texture::LoadTextureFromMemory(unsigned char* data, unsigned int size)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load_from_memory(data, size, &width, &height, &nrChannels, 0);

		if (!data) {
			std::cout << "Failed to load embedded texture." << std::endl;
			return false;
		}

		GLenum format;
		if (nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		return true;
	}
}