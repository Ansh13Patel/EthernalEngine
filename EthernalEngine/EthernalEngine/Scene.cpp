
#include"Scene.h"
#include "Model.h"

#include<iostream>

namespace EthernalEngine
{
	Scene::Scene(Window* window) :m_window(window), camera(window)
	{
		directionalLight = new DirectionalLight();
	}

	Scene::~Scene()
	{
		delete directionalLight;
	}

	void Scene::AddGameObject(GameObject* gameObject)
	{
		gameObjects.push_back(gameObject);
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
			defaultShader->LoadFromFile("Shader.vert", "Shader.frag");
		}

		defaultShader->LoadFromFile("Shader.vert", "Shader.frag");

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
			defaultShader->LoadFromFile("Shader.vert", "Shader.frag");
		}

		newObject->SetModel(newModel);
		newObject->SetShader(defaultShader);
		return newObject;
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
			if (RayAABB(camera.cameraPos, rayDir, minBounds, maxBounds, hitDistance)) 
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