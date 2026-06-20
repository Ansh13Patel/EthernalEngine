#pragma once

#include "Scene/GameObject.h"
#include "Scene/Camera.h"
#include "Rendering/Renderer.h"
#include "Rendering/CubeMesh.h"
#include "Core/Window.h"
#include "Scene/DirectionalLight.h"

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
		int GetGameObjectCount() const { return static_cast<int>(gameObjects.size()); }
		std::vector<GameObject*>& GetGameObjects() { return gameObjects; }
		void SelectGameObject(glm::vec3& rayDir);
		Camera& GetCamera() { return camera; }
		DirectionalLight* GetDirectionalLight() { return directionalLight; }
		GameObject* CreateCubeGameObject(std::string name);
		GameObject* CreateGameObjectWithCustomModel(std::string name, std::string path);
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
		Window* m_window = nullptr;
		GameObject* selectedGameObject = nullptr;
		Camera camera;
		CubeMesh* cubeMesh = nullptr;
		Shader* defaultShader = nullptr;
	};
}