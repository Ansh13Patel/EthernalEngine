#include"Scene/Scene.h"
#include "Rendering/Model.h"

#include<iostream>

namespace EthernalEngine
{
	Scene::Scene(Window* window) :m_window(window), EngineCamera(window)
	{
		directionalLight = nullptr;
	}

	Scene::~Scene()
	{
		delete directionalLight;
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
		this->selectedGameObject = gameObject;
	}

	GameObject* Scene::GetSelectedGameObject()
	{
		return selectedGameObject;
	}

	GameObject* Scene::CreateCubeGameObject(std::string name)
	{
		GameObject* newCube = new GameObject(name);
		if (cubeMesh == nullptr) 
		{
			std::shared_ptr<Texture> texture;
			texture = std::make_shared<Texture>();
			texture->LoadTextureFromPath("Textures/White.png");
			cubeMesh = new CubeMesh(texture);
		}
		if (defaultShader == nullptr)
		{
			defaultShader = new Shader();
			defaultShader->LoadFromFile("Shaders/Shader.vert", "Shaders/Shader.frag");
		}

		newCube->SetMesh(cubeMesh);
		newCube->SetShader(defaultShader);

		return newCube;
	}

	GameObject* Scene::CreateGameObjectWithCustomModel(std::string name, std::string path)
	{
		GameObject* newObject = new GameObject(name);
		
		Model* newModel = new Model(path, newObject);
		Texture* newTexture = new Texture();

		if (defaultShader == nullptr)
		{
			defaultShader = new Shader();
			defaultShader->LoadFromFile("Shaders/Shader.vert", "Shaders/Shader.frag");
		}

		newObject->SetModel(newModel);
		newObject->SetShader(defaultShader);
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

	void Scene::SelectGameObject(glm::vec3& rayDir)
	{
		float closetHitDistance = FLT_MAX;
		GameObject* selectedGameobject = nullptr;

		for (int i = 0; i < GetGameObjectCount(); i++)
		{
			Transform transform = gameObjects[i]->transform;
			glm::vec3 minBounds = transform.position - (transform.scale * 0.5f);
			glm::vec3 maxBounds = transform.position + (transform.scale * 0.5f);
			float hitDistance;
			if (RayAABB(EngineCamera.cameraPos, rayDir, minBounds, maxBounds, hitDistance)) 
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
			selectedGameObject = nullptr;
		}
	}

	bool Scene::RayAABB(const glm::vec3& rayOrgin, const glm::vec3& rayDir, const glm::vec3& minBounds,
		const glm::vec3& maxBounds, float& hitDistance)
	{
		glm::vec3 invDir = 1.0f / rayDir;

		float txMin = (minBounds.x - rayOrgin.x) * invDir.x;
		float txMax = (maxBounds.x - rayOrgin.x) * invDir.x;

		if(txMin > txMax)
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
}