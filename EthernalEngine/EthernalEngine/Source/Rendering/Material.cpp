#include "Rendering/Material.h"
#include "Components/DirectionalLight.h"
#include "Components/PointLight.h"
#include "Components/SpotLight.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"

#include <iostream>
#include <glad/glad.h>

namespace EthernalEngine
{
	void Material::Update(Scene& scene, GameObject* gameobject)
	{
		EngineCamera EngineCamera = scene.GetCamera();
		if (shader != nullptr)
		{
			shader->Use();
			shader->SetInt("image", 0);

			shader->SetMat4("view", EngineCamera.GetViewMatrix());
			shader->SetMat4("projection", EngineCamera.GetProjectionMatrix());
			if(gameobject != nullptr) shader->SetMat4("model", gameobject->transform->GetWorldMatrix());
			shader->SetFloat4("sceneAmbientColor", scene.GetAmbientColor());
			shader->SetFloat("sceneintensity", scene.GetIntensity());
			shader->SetFloat3("colorMultiplier", GetColor());
			shader->SetFloat("shininess", shininess);
			UpdateDirectionalLightOnObject(scene);
			UpdatePointLightsOnObject(scene);
			UpdateSpotLightsOnObject(scene);
			shader->SetFloat3("viewPos",
				std::vector<float>{EngineCamera.cameraPos.x, EngineCamera.cameraPos.y, EngineCamera.cameraPos.z}.data());
		}
		if (baseTexture != nullptr)
		{
			glActiveTexture(GL_TEXTURE0);
			baseTexture->Bind();
		}
	}

	void Material::UpdateDirectionalLightOnObject(Scene& scene)
	{
		DirectionalLight* dirLight = scene.GetDirectionalLight();
		if (dirLight != nullptr && dirLight->enable)
		{
			glm::vec3 forwardDir = dirLight->gameobject->transform->GetForward();
			// corrected uniform name to match shader struct (direction)
			shader->SetFloat3("directionalLight.direction", std::vector<float>{forwardDir.x, forwardDir.y, forwardDir.z}.data());
			shader->SetFloat("directionalLight.ambientStrength", dirLight->ambientStrength);
			shader->SetFloat("directionalLight.specularStrength", dirLight->specularStrength);
			shader->SetFloat("directionalLight.intensity", dirLight->intensity);
			shader->SetFloat4("directionalLight.color", dirLight->lightColor);
		}
	}

	void Material::UpdatePointLightsOnObject(Scene& scene)
	{
		std::vector<PointLight*> plLights = scene.GetPointLights();

		for (int i = 0; i < plLights.size(); i++)
		{
			PointLight* pl = plLights[i];
			glm::vec3 lightpos = pl->gameobject->transform->position;
			shader->SetFloat3(("pointLights[" + std::to_string(i) + "].pos").c_str(), std::vector<float>{lightpos.x, lightpos.y, lightpos.z}.data());
			shader->SetFloat(("pointLights[" + std::to_string(i) + "].specularStrength").c_str(), pl->specularStrength);
			shader->SetFloat(("pointLights[" + std::to_string(i) + "].intensity").c_str(), pl->intensity);
			shader->SetFloat(("pointLights[" + std::to_string(i) + "].radius").c_str(), pl->radius);
			shader->SetFloat4(("pointLights[" + std::to_string(i) + "].color").c_str(), pl->lightColor);
		}
		shader->SetInt("pointLightCount", plLights.size());
	}

	void Material::UpdateSpotLightsOnObject(Scene& scene)
	{
		std::vector<SpotLight*> slLights = scene.GetSpotLights();

		for (int i = 0; i < slLights.size(); i++)
		{
			SpotLight* sl = slLights[i];
			glm::vec3 lightpos = sl->gameobject->transform->position;
			glm::vec3 direction = sl->gameobject->transform->GetForward();
			shader->SetFloat3(("spotLights[" + std::to_string(i) + "].pos").c_str(), std::vector<float>{lightpos.x, lightpos.y, lightpos.z}.data());
			shader->SetFloat3(("spotLights[" + std::to_string(i) + "].direction").c_str(), std::vector<float>{direction.x, direction.y, direction.z}.data());
			shader->SetFloat(("spotLights[" + std::to_string(i) + "].specularStrength").c_str(), sl->specularStrength);
			shader->SetFloat(("spotLights[" + std::to_string(i) + "].intensity").c_str(), sl->intensity);
			shader->SetFloat(("spotLights[" + std::to_string(i) + "].range").c_str(), sl->range);
			shader->SetFloat(("spotLights[" + std::to_string(i) + "].angle").c_str(), sl->spotAngle);
			shader->SetFloat4(("spotLights[" + std::to_string(i) + "].color").c_str(), sl->lightColor);
		}
		shader->SetInt("spotLightCount", slLights.size());
	}

}