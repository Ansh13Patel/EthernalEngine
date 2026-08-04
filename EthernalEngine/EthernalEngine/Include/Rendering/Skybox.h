#pragma once

#include "Rendering/Shader.h"
#include "Rendering/CubeMap.h"

#include <memory>
#include <vector>

namespace EthernalEngine
{
    class EngineCamera;
	class ICamera;
	class Skybox 
	{
	public:
		Skybox();
		~Skybox() = default;
		void SetupSkyboxUsingCubemap(const CubeMapFace& faces);
		void SetupProceduralSkybox();
        void Draw(const ICamera& camera);

	private:
		unsigned int skyboxVAO, skyboxVBO;
		std::unique_ptr<Shader> skyboxShader;
		std::unique_ptr<CubeMap> cubeMap = nullptr;
		std::vector<float> skyboxVertices;
	};
}