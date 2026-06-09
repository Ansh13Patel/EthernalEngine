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
		Camera& GetCamera() { return camera; }
		GameObject* CreateCubeGameObject(std::string name);
		GameObject* GetSelectedGameObject();
		void SetSelectedGameObject(GameObject* gameObject);
		CubeMesh* GetCubeMesh() { return cubeMesh; }
		Shader* GetCubeShader() { return cubeShader; }
		Texture* GetCubeTexture() { return cubeTexture; }

	private:
		std::vector<GameObject*> gameObjects;
		GameObject* selectedGameObject = nullptr;
		Camera camera{ 800.0f, 600.0f };
		CubeMesh* cubeMesh = nullptr;
		Shader* cubeShader = nullptr;
		Texture* cubeTexture = nullptr;
	};
}