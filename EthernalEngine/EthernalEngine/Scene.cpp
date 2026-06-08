
#include"Scene.h"

namespace EthernalEngine
{
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

	GameObject* Scene::CreateCubeGameObject(std::string name)
	{
		GameObject* newCube = new GameObject(name);
		if (cubeMesh == nullptr) 
		{
			cubeMesh = new CubeMesh();
		}
		if (cubeShader == nullptr)
		{
			cubeShader = new Shader();
		}
		if (cubeTexture == nullptr)
		{
			cubeTexture = new Texture();
		}

		cubeShader->LoadFromFile("Shader.vert", "Shader.frag");
		cubeTexture->LoadTexture("Textures/White.png");

		newCube->SetMesh(cubeMesh);
		newCube->SetShader(cubeShader);
		newCube->SetTexture(cubeTexture);

		return newCube;
	}
}