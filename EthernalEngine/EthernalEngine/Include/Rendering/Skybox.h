#pragma once

#include "Rendering/Shader.h"
#include "Rendering/CubeMap.h"

#include <memory>
#include <vector>

namespace EthernalEngine
{
	class EngineCamera;
	class Skybox 
	{
	public:
		Skybox();
		~Skybox() = default;
		void SetupSkybox(const CubeMapFace& faces);
		void Draw(EngineCamera& camera);

	private:
		unsigned int skyboxVAO, skyboxVBO;
		std::unique_ptr<Shader> skyboxShader;
		std::unique_ptr<CubeMap> cubeMap;
		std::vector<float> skyboxVertices;
	};
}