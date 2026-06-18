#include "Renderer.h"
#include "Scene.h"

namespace EthernalEngine
{
	void Renderer::Clear()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::Draw(Scene* scene)
	{
		std::vector<GameObject*>& gameobjects = scene->GetGameObjects();
		Camera* camera = &scene->GetCamera();
		DirectionalLight* dirLight = scene->GetDirectionalLight();

		Shader* currentShader = nullptr;
		for (GameObject* obj : gameobjects)
		{
			if (obj)
			{
				if (obj->GetShader() != currentShader && camera != nullptr)
				{
					obj->GetShader()->Use();
					obj->GetShader()->SetMat4("view", camera->GetViewMatrix());
					obj->GetShader()->SetMat4("projection", camera->GetProjectionMatrix());
					if (dirLight != nullptr && dirLight->useLight)
					{
						obj->GetShader()->SetFloat3("lightDirection", std::vector<float>{-0.2f, -1.0f, -0.3f}.data());
						obj->GetShader()->SetFloat("ambientStrength", dirLight->ambientStrength);
						obj->GetShader()->SetFloat("specularStrength", dirLight->specularStrength);
						obj->GetShader()->SetFloat("intensity", dirLight->intensity);
						obj->GetShader()->SetFloat3("lightColor", dirLight->lightColor);
					}
					obj->GetShader()->SetFloat3("viewPos",
						std::vector<float>{camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z}.data());

					currentShader = obj->GetShader();
				}
				obj->Draw();
			}
		}
	}
}