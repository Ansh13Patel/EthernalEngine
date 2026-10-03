#include "Scene/Scene.h"
#include "Rendering/Model.h"
#include "Helper/DebugDraw.h"
#include "Editor/EditorUI.h"
#include "Components/Camera.h"

#include<iostream>


namespace EthernalEngine
{
	Scene::Scene(Window* window, Viewport& sceneViewport, Viewport& gameViewport) : m_window(window), 
		engineCam(sceneViewport), m_gameViewport(gameViewport)
	{
		pendingSceneLoad = false;
		directionalLight = nullptr;
		skybox = new Skybox();
		skybox->SetupProceduralSkybox();

		if (defaultShader == nullptr)
		{
			defaultShader = new Shader();
			defaultShader->LoadFromFile("Shaders/Shader.vert", "Shaders/Shader.frag");
		}
		if (cubeMesh == nullptr)
		{
			cubeMesh = new CubeMesh();
		}
		if(planeMesh == nullptr)
		{
			planeMesh = new PlaneMesh();
		}
		if(sphereMesh == nullptr)
		{
			sphereMesh = new SphereMesh(0.5f, 32, 16);
		}

		sceneBuffer = new FrameBuffer();
		sceneBuffer->Create(window->GetWidth(), window->GetHeight());
		DebugDraw::Init();

		BaseSceneSetup();
	}

	Scene::~Scene()
	{
		ClearScene();
	}

	void Scene::BaseSceneSetup()
	{
		AddCamera(CreateGameObjectWithCamera());
		AddDirectionalLight(CreateGameObjectWithDirectionalLight());
	}

	void Scene::AddGameObject(GameObject* gameObject)
	{
		gameObjects.push_back(gameObject);
	}

	void Scene::AddDirectionalLight(DirectionalLight* directionalLight)
	{
		if (this->directionalLight == nullptr)
		{
			this->directionalLight = directionalLight;
		}
	}

	void Scene::AddPointLight(PointLight* pointLight)
	{
		this->pointLights.push_back(pointLight);
	}

	void Scene::AddSpotLight(SpotLight* spotLight)
	{
		this->spotLights.push_back(spotLight);
	}

	void Scene::AddCamera(Camera* cam)
	{
		if (this->mainCamera == nullptr)
		{
			this->mainCamera = cam;
		}
	}

	void Scene::Update(float deltaTime)
	{
		for (GameObject* obj : gameObjects)
		{
			if (obj)
			{
				obj->Update(deltaTime);
			}
		}
	}

	void Scene::SetSelectedGameObject(GameObject* gameObject)
	{
		if (this->selectedGameObject != gameObject)
		{
			if (this->selectedGameObject != nullptr)
				this->selectedGameObject->SetIsSelected(false);

			this->selectedGameObject = gameObject;

			if (this->selectedGameObject != nullptr)
				this->selectedGameObject->SetIsSelected(true);
		}
	}

	GameObject* Scene::GetSelectedGameObject()
	{
		return selectedGameObject;
	}

	GameObject* Scene::CreateDefaultGameObject(std::string name, DefaultMeshType type)
	{
		GameObject* newObject = new GameObject(name);

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->LoadTextureFromPath("Textures/White.png");

		Material* mat = nullptr;
		mat = new Material();
		mat->SetShader(defaultShader);
		mat->SetBaseTexture(texture);

		newObject->defaultMeshType = type;
		switch (type)
		{
		case DefaultMeshType::Cube:
			newObject->SetMesh(cubeMesh);
			break;
		case DefaultMeshType::Plane:
			newObject->SetMesh(planeMesh);
			break;
		case DefaultMeshType::Sphere:
			newObject->SetMesh(sphereMesh);
			break;
		default:
			break;
		}

		newObject->SetMaterial(mat);
		return newObject;
	}

	GameObject* Scene::CreateGameObjectWithCustomModel(std::string name, std::string path)
	{
		GameObject* newObject = new GameObject(name);
		newObject->modelPath = path;

		Model* newModel = new Model(path, newObject, defaultShader);

		return newObject;
	}

	DirectionalLight* Scene::CreateGameObjectWithDirectionalLight()
	{
		GameObject* dirLightObj = new GameObject("Directional Light");
		DirectionalLight* dirLight = new DirectionalLight(dirLightObj);
		dirLightObj->components.push_back(dirLight);

		AddGameObject(dirLightObj);

		return dirLight;
	}

	PointLight* Scene::CreateGameObjectWithPointLight()
	{
		GameObject* pointLightObj = new GameObject("Point Light");
		PointLight* pointLight = new PointLight(pointLightObj);
		pointLightObj->components.push_back(pointLight);

		AddGameObject(pointLightObj);

		return pointLight;
	}

	SpotLight* Scene::CreateGameObjectWithSpotLight()
	{
		GameObject* spotLightObj = new GameObject("Spot Light");
		SpotLight* spotLight = new SpotLight(spotLightObj);
		spotLightObj->components.push_back(spotLight);

		AddGameObject(spotLightObj);

		spotLightObj->transform->rotation = glm::quat(glm::radians(glm::vec3(-90.0f, 0.0f, 0.0f)));

		return spotLight;
	}

	Camera* Scene::CreateGameObjectWithCamera()
	{
		GameObject* cameraObj = new GameObject("Main Camera");
		Camera* cam = new Camera(cameraObj,m_window, m_gameViewport);
		cameraObj->AddComponent(cam);

		AddGameObject(cameraObj);

		return cam;
	}

	void Scene::SelectGameObject(glm::vec3& rayDir)
	{
		float closetHitDistance = FLT_MAX;
		GameObject* selectedGameobject = nullptr;

		for (int i = 0; i < GetGameObjectCount(); i++)
		{
			Transform* transform = gameObjects[i]->transform;
			glm::vec3 minBounds = transform->position - (transform->scale * 0.5f);
			glm::vec3 maxBounds = transform->position + (transform->scale * 0.5f);
			float hitDistance;
            if (RayAABB(engineCam.transform.position, rayDir, minBounds, maxBounds, hitDistance))
			{
				if (hitDistance < closetHitDistance)
				{
					selectedGameobject = gameObjects[i];
					closetHitDistance = hitDistance;
				}
			}
		}

		if (selectedGameobject)
		{
			SetSelectedGameObject(selectedGameobject);
		}
		else
		{
			SetSelectedGameObject(nullptr);
		}
	}

	bool Scene::RayAABB(const glm::vec3& rayOrgin, const glm::vec3& rayDir, const glm::vec3& minBounds,
		const glm::vec3& maxBounds, float& hitDistance)
	{
		glm::vec3 invDir = 1.0f / rayDir;

		float txMin = (minBounds.x - rayOrgin.x) * invDir.x;
		float txMax = (maxBounds.x - rayOrgin.x) * invDir.x;

		if (txMin > txMax)
		{
			std::swap(txMin, txMax);
		}

		float tyMin = (minBounds.y - rayOrgin.y) * invDir.y;
		float tyMax = (maxBounds.y - rayOrgin.y) * invDir.y;

		if (tyMin > tyMax)
		{
			std::swap(tyMin, tyMax);
		}

		if (txMin > tyMax || tyMin > txMax) return false;

		float tMin = std::max(txMin, tyMin);
		float tMax = std::min(txMax, tyMax);

		float tzMin = (minBounds.z - rayOrgin.z) * invDir.z;
		float tzMax = (maxBounds.z - rayOrgin.z) * invDir.z;

		if (tzMin > tzMax)
		{
			std::swap(tzMin, tzMax);
		}

		if (tMin > tzMax || tzMin > tMax) return false;

		tMin = std::max(tMin, tzMin);

		hitDistance = tMin;

		return true;
	}

	json Scene::SerializeScene() const
	{
		json sceneJson;
		sceneJson["ambientColor"] = { ambientColor[0], ambientColor[1], ambientColor[2], ambientColor[3] };
		sceneJson["intensity"] = intensity;
		if (!gameObjects.empty())
		{
			sceneJson["gameObjects"] = json::array();
		}
		for (const auto& gameObject : gameObjects)
		{
			sceneJson["gameObjects"].push_back(gameObject->SerializeGameObject());
		}
		return sceneJson;
	}

	void Scene::DeserializeScene(const json& sceneJson)
	{
		selectedGameObject = nullptr;
		if (sceneJson.contains("ambientColor"))
		{
			auto col = sceneJson["ambientColor"];
			ambientColor[0] = col[0];
			ambientColor[1] = col[1];
			ambientColor[2] = col[2];
			ambientColor[3] = col[3];
		}
		if (sceneJson.contains("intensity"))
		{
			intensity = sceneJson["intensity"].get<float>();
		}
		if (sceneJson.contains("gameObjects"))
		{
			for (const auto& gameObjectJson : sceneJson["gameObjects"])
			{
				GameObject* newGameObject = new GameObject("Deserialized Object");
				newGameObject->DeserializeGameObject(gameObjectJson);
				if (newGameObject->modelPath != "")
				{
					Model* newModel = new Model(newGameObject->modelPath, newGameObject, defaultShader);
				}
				else
				{
					switch (newGameObject->defaultMeshType)
					{
					case DefaultMeshType::Cube:
						newGameObject->SetMesh(cubeMesh);
						break;
					default:
						break;
					}
				}
				AddGameObject(newGameObject);
			}
		}

		AddAllLights();
	}

	void Scene::LoadPendingScene()
	{
		ClearScene();
		DeserializeScene(pendingSceneData);
		pendingSceneLoad = false;
	}

	bool Scene::ClearScene()
	{
		for (GameObject* obj : gameObjects)
		{
			if (obj != nullptr)
			{
				delete obj;
			}
		}

		directionalLight = nullptr;

		gameObjects.clear();
		pointLights.clear();
		spotLights.clear();

		return true;
	}

    void Scene::AddAllLights()
    {
        for (GameObject* obj : gameObjects)
        {
			if (obj != nullptr)
			{
				for (Component* comp : obj->components)
				{
					if (comp != nullptr)
					{
						if (auto* dir = dynamic_cast<DirectionalLight*>(comp))
						{
							if (dir != nullptr)
								directionalLight = dir;
						}
						else if (auto* pl = dynamic_cast<PointLight*>(comp))
						{
							if (pl != nullptr)
								pointLights.push_back(pl);
						}
						else if (auto* sl = dynamic_cast<SpotLight*>(comp))
						{
							if (sl != nullptr)
								spotLights.push_back(sl);
						}
					}
				}
			}
        }
    }
}