
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
		Shader shader;
		shader.LoadFromFile("Shader.vert", "Shader.frag");
		Texture texture;
		texture.LoadTexture("Textures/White.png");

		newCube->SetMesh(cubeMesh);
		newCube->SetShader(&shader);
		newCube->SetTexture(&texture);

		return newCube;
	}
}