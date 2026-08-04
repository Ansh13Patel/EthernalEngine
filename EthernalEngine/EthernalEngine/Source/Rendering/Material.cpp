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
	void Material::Update(Scene& scene, GameObject* gameobject, ICamera* cam)
	{
		if (shader != nullptr)
		{
			shader->Use();
			shader->SetInt("image", 0);

			shader->SetMat4("view", cam->GetViewMatrix());
			shader->SetMat4("projection", cam->GetProjectionMatrix());
			if(gameobject != nullptr) shader->SetMat4("model", gameobject->transform->GetWorldMatrix());
			shader->SetFloat4("sceneAmbientColor", scene.GetAmbientColor());
			shader->SetFloat("sceneintensity", scene.GetIntensity());
			shader->SetFloat3("baseColor", GetColor());
			shader->SetFloat("shininess", shininess);
			shader->SetFloat("metallic", metallic);
			shader->SetFloat("transparency", transparency);
			shader->SetFloat("ior", ior);
			shader->SetFloat("roughness", roughness);	
		    UpdateDirectionalLightOnObject(scene);
			UpdatePointLightsOnObject(scene);
			UpdateSpotLightsOnObject(scene);
			shader->SetFloat3("viewPos",
				std::vector<float>{cam->GetPosition().x, cam->GetPosition().y, cam->GetPosition().z}.data());
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
			glm::vec3 forwardDir = dirLight->parentObj->transform->GetForward();
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
			glm::vec3 lightpos = pl->parentObj->transform->position;
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
			glm::vec3 lightpos = sl->parentObj->transform->position;
			glm::vec3 direction = sl->parentObj->transform->GetForward();
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

	json Material::SerializeMaterial() const
	{
		json materialJson;
		if (baseTexture != nullptr)
		{
			materialJson["texture"] = baseTexture->SerializeTexture();
		}
		if (shader != nullptr)
		{
			materialJson["shader"] = shader->SerializeShader();
		}
		materialJson["color"] = { color[0], color[1], color[2] };
		materialJson["shininess"] = shininess;
		materialJson["metallic"] = metallic;
		materialJson["transparency"] = transparency;
		materialJson["roughness"] = roughness;
		materialJson["ior"] = ior;
		return materialJson;
	}

	void Material::DeserializeMaterial(const json& materialJson)
	{
		if (materialJson.contains("color") && materialJson["color"].is_array() && materialJson["color"].size() == 3)
		{
			color[0] = materialJson["color"][0].get<float>();
			color[1] = materialJson["color"][1].get<float>();
			color[2] = materialJson["color"][2].get<float>();
		}
		if (materialJson.contains("shininess")) shininess = materialJson["shininess"].get<float>();
		if (materialJson.contains("metallic")) metallic = materialJson["metallic"].get<float>();
		if (materialJson.contains("transparency")) transparency = materialJson["transparency"].get<float>();
		if (materialJson.contains("roughness")) roughness = materialJson["roughness"].get<float>();
		if (materialJson.contains("ior")) ior = materialJson["ior"].get<float>();
		if (materialJson.contains("texture") && materialJson["texture"].is_object())
		{
			json textureJson = materialJson["texture"];
			if (!textureJson.empty())
			{
				baseTexture = std::make_shared<Texture>();
				baseTexture->DeserializeTexture(textureJson);
			}
		}
		if(materialJson.contains("shader") && materialJson["shader"].is_object())
		{
			json shaderJson = materialJson["shader"];
			if (!shaderJson.empty())
			{
				shader = new Shader();
				shader->DeserializeShader(shaderJson);
			}
		}
	}
}