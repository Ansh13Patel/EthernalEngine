#include "Rendering/Skybox.h"
#include "Core/EngineCamera.h"

#include <algorithm>
#include <vector>

namespace EthernalEngine
{
	Skybox::Skybox()
	{
		const float vertices[108] = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		skyboxVertices.assign(vertices, vertices + 108);
	}

	void Skybox::SetupSkyboxUsingCubemap(const CubeMapFace& faces)
	{
		cubeMap = std::make_unique<CubeMap>();	
		cubeMap->LoadCubeMap(faces);

		skyboxShader = std::make_unique<Shader>();
		skyboxShader->LoadFromFile("Shaders/SkyboxShader.vert", "Shaders/SkyboxShader.frag");

		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);

		glBindVertexArray(skyboxVAO);

		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

		glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), skyboxVertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	void Skybox::SetupProceduralSkybox()
	{
		skyboxShader = std::make_unique<Shader>();
		skyboxShader->LoadFromFile("Shaders/ProceduralSkybox.vert", "Shaders/ProceduralSkybox.frag");

		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);

		glBindVertexArray(skyboxVAO);

		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

		glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), skyboxVertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	void Skybox::Draw(const ICamera& camera)
	{
		glDepthFunc(GL_LEQUAL);

		skyboxShader->Use();

		glm::mat4 view =
			glm::mat4(glm::mat3(camera.GetViewMatrix()));

		skyboxShader->SetMat4("view", view);
		skyboxShader->SetMat4("projection",
			camera.GetProjectionMatrix());

		glBindVertexArray(skyboxVAO);

		if(cubeMap != nullptr)
			cubeMap->Bind(GL_TEXTURE2);
		else
		{
			skyboxShader->SetFloat3("uSkyColor", std::vector<float>{0.25f, 0.50f, 0.95f}.data());
			skyboxShader->SetFloat3("uHorizonColor", std::vector<float>{0.85f, 0.90f, 1.0f}.data());
			skyboxShader->SetFloat3("uGroundColor", std::vector<float>{0.35f, 0.35f, 0.40f}.data());
		}

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}
}