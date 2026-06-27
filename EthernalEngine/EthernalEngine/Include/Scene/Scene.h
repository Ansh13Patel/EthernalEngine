#pragma once

#include "Scene/GameObject.h"
#include "Core/EngineCamera.h"
#include "Rendering/Renderer.h"
#include "Rendering/CubeMesh.h"
#include "Core/Window.h"
#include "Components/DirectionalLight.h"
#include "Components/PointLight.h"
#include "Components/SpotLight.h"

#include <vector>

namespace EthernalEngine
{
	class Scene
	{
	public:
		Scene(Window* window);
		~Scene();
		void Update(float deltaTime);
		void AddGameObject(GameObject* gameObject);
		void AddDirectionalLight(DirectionalLight* directioanllight);
		void AddPointLight(PointLight* pointlight);
		void AddSpotLight(SpotLight* spotlight);
		int GetGameObjectCount() const { return static_cast<int>(gameObjects.size()); }
		std::vector<GameObject*>& GetGameObjects() { return gameObjects; }
		void SelectGameObject(glm::vec3& rayDir);
		EngineCamera& GetCamera() { return EngineCamera; }
		DirectionalLight* GetDirectionalLight() { return directionalLight; }
		std::vector<PointLight*> GetPointLights() { return pointLights; }
		std::vector<SpotLight*> GetSpotLights() { return spotLights; }
		GameObject* CreateCubeGameObject(std::string name);
		GameObject* CreateGameObjectWithCustomModel(std::string name, std::string path);
		DirectionalLight* CreateGameObjectWithDirectionalLight();
		PointLight* CreateGameObjectWithPointLight();
		SpotLight* CreateGameObjectWithSpotLight();
		GameObject* GetSelectedGameObject();
		void SetSelectedGameObject(GameObject* gameObject);
		CubeMesh* GetCubeMesh() { return cubeMesh; }
		Shader* GetCubeShader() { return defaultShader; }

	private:
		bool RayAABB(const glm::vec3& rayOrgin, const glm::vec3& rayDir, const glm::vec3& minBounds,
			const glm::vec3& maxBounds, float& hitDistance);
	private:
		std::vector<GameObject*> gameObjects;
		DirectionalLight* directionalLight;
		std::vector<PointLight*> pointLights;
		std::vector<SpotLight*> spotLights;
		Window* m_window = nullptr;
		GameObject* selectedGameObject = nullptr;
		EngineCamera EngineCamera;
		CubeMesh* cubeMesh = nullptr;
		Shader* defaultShader = nullptr;
	};
}