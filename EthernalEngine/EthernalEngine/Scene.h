#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Renderer.h"
#include "CubeMesh.h"
#include <vector>

namespace EthernalEngine
{
	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;
		void Update(float deltaTime);
		void AddGameObject(GameObject* gameObject);
		int GetGameObjectCount() const { return static_cast<int>(gameObjects.size()); }
		std::vector<GameObject*>& GetGameObjects() { return gameObjects; }
		void SelectGameObject(glm::vec3& rayDir);
		Camera& GetCamera() { return camera; }
		GameObject* CreateCubeGameObject(std::string name);
		GameObject* GetSelectedGameObject();
		void SetSelectedGameObject(GameObject* gameObject);
		CubeMesh* GetCubeMesh() { return cubeMesh; }
		Shader* GetCubeShader() { return cubeShader; }

	private:
		bool RayAABB(const glm::vec3& rayOrgin, const glm::vec3& rayDir, const glm::vec3& minBounds,
			const glm::vec3& maxBounds, float& hitDistance);
	private:
		std::vector<GameObject*> gameObjects;
		GameObject* selectedGameObject = nullptr;
		Camera camera{ 800.0f, 600.0f };
		CubeMesh* cubeMesh = nullptr;
		Shader* cubeShader = nullptr;
	};
}