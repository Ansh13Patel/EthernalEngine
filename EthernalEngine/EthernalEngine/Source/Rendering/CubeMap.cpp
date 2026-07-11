#include "Rendering/CubeMap.h"

#include <iostream>
namespace EthernalEngine
{
	CubeMap::CubeMap()
	{
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void CubeMap::LoadCubeMap(const CubeMapFace& faces)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
		const std::string facePaths[6] = {
			faces.Cube_Map_Positive_X,
			faces.Cube_Map_Negative_X,
			faces.Cube_Map_Positive_Y,
			faces.Cube_Map_Negative_Y,
			faces.Cube_Map_Positive_Z,
			faces.Cube_Map_Negative_Z
		};
		for(int i = 0; i < 6; i++)
		{
			int width, height, nrChannels;
			unsigned char* data = stbi_load(facePaths[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Failed to load cubemap texture at path: " << facePaths[i] << std::endl;
				stbi_image_free(data);
			}
		}
	}

	void CubeMap::Bind(GLenum textureUnit)
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	}
}