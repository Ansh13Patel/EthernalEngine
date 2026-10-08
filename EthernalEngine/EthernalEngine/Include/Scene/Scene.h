#pragma once

#include "Scene/GameObject.h"
#include "Core/EngineCamera.h"
#include "Rendering/Renderer.h"
#include "Rendering/CubeMesh.h"
#include "Rendering/PlaneMesh.h"
#include "Rendering/SphereMesh.h"
#include "Rendering/CylinderMesh.h"
#include "Rendering/Material.h"
#include "Rendering/Skybox.h"
#include "Core/Window.h"
#include "Components/DirectionalLight.h"
#include "Components/PointLight.h"
#include "Components/SpotLight.h"
#include "Rendering/FrameBuffer.h"

#include <vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace EthernalEngine
{
	struct Viewport;
	class Camera;
	class Scene
	{
	public:
		Scene(Window* window, Viewport& sceneViewport, Viewport& gameViewport);
		~Scene();
		void BaseSceneSetup();
		void Update(float deltaTime);
		void AddGameObject(GameObject* gameObject);
		void AddDirectionalLight(DirectionalLight* directioanllight);
		void AddPointLight(PointLight* pointlight);
		void AddSpotLight(SpotLight* spotlight);
		void AddCamera(Camera* cam);
		int GetGameObjectCount() const { return static_cast<int>(gameObjects.size()); }
		std::vector<GameObject*>& GetGameObjects() { return gameObjects; }
		void SelectGameObject(glm::vec3& rayDir);
		EngineCamera& GetCamera() { return engineCam; }
		DirectionalLight* GetDirectionalLight() { return directionalLight; }
		std::vector<PointLight*> GetPointLights() { return pointLights; }
		std::vector<SpotLight*> GetSpotLights() { return spotLights; }
		GameObject* CreateDefaultGameObject(std::string name, DefaultMeshType type = DefaultMeshType::None);
		GameObject* CreateGameObjectWithCustomModel(std::string name, std::string path);
		DirectionalLight* CreateGameObjectWithDirectionalLight();
		PointLight* CreateGameObjectWithPointLight();
		SpotLight* CreateGameObjectWithSpotLight();
		Camera* CreateGameObjectWithCamera();
		GameObject* GetSelectedGameObject();
		Skybox* GetSkybox() { return skybox; }
		FrameBuffer* GetSceneBuffer() { return sceneBuffer; }
		Camera* GetMainCamera() { return mainCamera; }
		void SetSelectedGameObject(GameObject* gameObject);
		CubeMesh* GetCubeMesh() { return cubeMesh; }
		Shader* GetCubeShader() { return defaultShader; }
		float* GetAmbientColor() { return ambientColor; }
		float GetIntensity() { return intensity; }
		json SerializeScene() const;
		void DeserializeScene(const json& sceneJson);
		void LoadPendingScene();
		bool pendingSceneLoad = false;
		json pendingSceneData;

	private:
		bool RayAABB(const glm::vec3& rayOrgin, const glm::vec3& rayDir, const glm::vec3& minBounds,
			const glm::vec3& maxBounds, float& hitDistance);
		bool ClearScene();
		void AddAllLights();

	private:
		FrameBuffer* sceneBuffer = nullptr;
		Viewport& m_gameViewport;
		Camera* mainCamera = nullptr;
		std::vector<GameObject*> gameObjects;
		DirectionalLight* directionalLight;
		std::vector<PointLight*> pointLights;
		std::vector<SpotLight*> spotLights;
		Window* m_window = nullptr;
		GameObject* selectedGameObject = nullptr;
		EngineCamera engineCam;
		Skybox* skybox = nullptr;
		CubeMesh* cubeMesh = nullptr;
		SphereMesh* sphereMesh = nullptr;
		PlaneMesh* planeMesh = nullptr;
		CylinderMesh* cylinderMesh = nullptr;
		Shader* defaultShader = nullptr;
		float ambientColor[4]{ 1.0f,1.0f,1.0f,1.0f };
		float intensity = 0.2f;
	};
}