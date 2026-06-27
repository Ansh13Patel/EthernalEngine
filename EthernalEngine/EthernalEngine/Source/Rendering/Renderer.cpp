#include "Rendering/Renderer.h"
#include "Scene/Scene.h"

#include <iostream>
namespace EthernalEngine
{
	void Renderer::Clear()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::Draw(Scene* scene)
	{
		std::vector<GameObject*>& gameobjects = scene->GetGameObjects();
		EngineCamera* EngineCamera = &scene->GetCamera();
		DirectionalLight* dirLight = scene->GetDirectionalLight();
		std::vector<PointLight*> plLights = scene->GetPointLights();

		Shader* currentShader = nullptr;
		for (GameObject* obj : gameobjects)
		{
			if (obj)
			{
				Shader* objShader = obj->GetShader();
				if (objShader != nullptr && objShader != currentShader && EngineCamera != nullptr)
				{
					obj->GetShader()->Use();
					obj->GetShader()->SetMat4("view", EngineCamera->GetViewMatrix());
					obj->GetShader()->SetMat4("projection", EngineCamera->GetProjectionMatrix());
					UpdateDirectionalLightOnObject(scene, obj);
					UpdatePointLightsOnObject(scene, obj);
					UpdateSpotLightsOnObject(scene, obj);
					obj->GetShader()->SetFloat3("viewPos",
						std::vector<float>{EngineCamera->cameraPos.x, EngineCamera->cameraPos.y, EngineCamera->cameraPos.z}.data());

					currentShader = obj->GetShader();
				}
				obj->Draw();
			}
		}
	}

	void Renderer::UpdateDirectionalLightOnObject(Scene* scene,GameObject* obj)
	{
		DirectionalLight* dirLight = scene->GetDirectionalLight();
		if (dirLight != nullptr && dirLight->enable)
		{
			glm::vec3 forwardDir = dirLight->gameobject->transform.GetForward();
			obj->GetShader()->SetFloat3("directionalLight.lightDirection", std::vector<float>{forwardDir.x, forwardDir.y, forwardDir.z}.data());
			obj->GetShader()->SetFloat("directionalLight.ambientStrength", dirLight->ambientStrength);
			obj->GetShader()->SetFloat("directionalLight.specularStrength", dirLight->specularStrength);
			obj->GetShader()->SetFloat("directionalLight.intensity", dirLight->intensity);
			obj->GetShader()->SetFloat4("directionalLight.color", dirLight->lightColor);
		}
	}

	void Renderer::UpdatePointLightsOnObject(Scene* scene, GameObject* obj)
	{
		std::vector<PointLight*> plLights = scene->GetPointLights();

		for (int i = 0; i < plLights.size(); i++)
		{
			PointLight* pl = plLights[i];
			glm::vec3 lightpos = pl->gameobject->transform.position;
			obj->GetShader()->SetFloat3(("pointLights[" + std::to_string(i) + "].pos").c_str(), std::vector<float>{lightpos.x, lightpos.y, lightpos.z}.data());
			obj->GetShader()->SetFloat(("pointLights[" + std::to_string(i) + "].specularStrength").c_str(), pl->specularStrength);
			obj->GetShader()->SetFloat(("pointLights[" + std::to_string(i) + "].intensity").c_str(), pl->intensity);
			obj->GetShader()->SetFloat(("pointLights[" + std::to_string(i) + "].radius").c_str(), pl->radius);
			obj->GetShader()->SetFloat4(("pointLights[" + std::to_string(i) + "].color").c_str(), pl->lightColor);
		}
		obj->GetShader()->SetInt("pointLightCount", plLights.size());
	}

	void Renderer::UpdateSpotLightsOnObject(Scene* scene, GameObject* obj)
	{
		std::vector<SpotLight*> slLights = scene->GetSpotLights();

		for (int i = 0; i < slLights.size(); i++)
		{
			SpotLight* sl = slLights[i];
			glm::vec3 lightpos = sl->gameobject->transform.position;
			glm::vec3 direction = sl->gameobject->transform.GetForward();
			obj->GetShader()->SetFloat3(("spotLights[" + std::to_string(i) + "].pos").c_str(), std::vector<float>{lightpos.x, lightpos.y, lightpos.z}.data());
			obj->GetShader()->SetFloat3(("spotLights[" + std::to_string(i) + "].direction").c_str(), std::vector<float>{direction.x, direction.y, direction.z}.data());
			obj->GetShader()->SetFloat(("spotLights[" + std::to_string(i) + "].specularStrength").c_str(), sl->specularStrength);
			obj->GetShader()->SetFloat(("spotLights[" + std::to_string(i) + "].intensity").c_str(), sl->intensity);
			obj->GetShader()->SetFloat(("spotLights[" + std::to_string(i) + "].range").c_str(), sl->range);
			obj->GetShader()->SetFloat(("spotLights[" + std::to_string(i) + "].angle").c_str(), sl->spotAngle);
			obj->GetShader()->SetFloat4(("spotLights[" + std::to_string(i) + "].color").c_str(), sl->lightColor);
		}
		obj->GetShader()->SetInt("spotLightCount", slLights.size());
	}
}