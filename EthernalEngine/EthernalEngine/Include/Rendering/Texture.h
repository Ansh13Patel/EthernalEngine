#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

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
		json SerializeTexture() const;
		void DeserializeTexture(const json& textureJson);

	public:
		std::string path = "";

	private:
		unsigned int m_textureID;
		unsigned char* data;
		int width, height, nrChannels;
	};
}