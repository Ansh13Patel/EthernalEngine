#pragma once

#include "Rendering/Shader.h"
#include "Rendering/Texture.h"

#include <memory>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace EthernalEngine
{
	class Scene;
	class GameObject;

	class Material 
	{
	public:
		Material() = default;
		~Material() = default;
		void Update(Scene& scene, GameObject* gameobject);
		void SetShader(Shader* newShader) { shader = newShader; }
		void SetBaseTexture(std::shared_ptr<Texture> newTexture) { baseTexture = newTexture; }
		Shader* GetShader() const { return shader; }
		std::shared_ptr<Texture> GetBaseTexture() { return baseTexture; }
		float* GetColor() { return color; }
		json SerializeMaterial() const;
		void DeserializeMaterial(const json& materialJson);

	private:
		void UpdateDirectionalLightOnObject(Scene& scene);
		void UpdatePointLightsOnObject(Scene& scene);
		void UpdateSpotLightsOnObject(Scene& scene);

	public:
		Shader* shader = nullptr;
		std::shared_ptr<Texture> baseTexture;
		float color[3]{ 1.0f,1.0f,1.0f};
		float shininess = 32.0f;
		float metallic = 0.5f;
		float transparency = 0.5f;
		float roughness = 0.0f;
		float ior = 1.52f;
	};
}