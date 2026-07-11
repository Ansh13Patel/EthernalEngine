#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <vector>
#include <string>

namespace EthernalEngine
{
	struct CubeMapFace
	{
		std::string Cube_Map_Positive_X;
		std::string Cube_Map_Negative_X;
		std::string Cube_Map_Positive_Y;
		std::string Cube_Map_Negative_Y;
		std::string Cube_Map_Positive_Z;
		std::string Cube_Map_Negative_Z;
	};
	class CubeMap
	{
	public:
		CubeMap();
		~CubeMap() = default;
		void LoadCubeMap(const CubeMapFace& faces);
		void Bind(GLenum textureUnit);
		unsigned int GetTextureID() const { return m_textureID; }

	private:
		unsigned int m_textureID;
	};
}
