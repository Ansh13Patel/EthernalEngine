#include "Rendering/Renderer.h"
#include "Scene/Scene.h"
#include "Helper/DebugDraw.h"

#include <iostream>

namespace EthernalEngine
{
	void Renderer::RenderGameObjectRecursive(GameObject* obj, Scene& scene, ICamera* cam)
	{
		if (!obj) return;

		Material* mat = obj->GetMaterial();
		if (mat != nullptr)
		{
			shadowMap.BindTexture(GL_TEXTURE5);
			mat->Update(scene, obj, cam);
			obj->Draw();
		}


		for (GameObject* child : obj->childObjects)
		{
			RenderGameObjectRecursive(child, scene, cam);
		}
	}

	void Renderer::RenderGameObjectShadowRecursive(GameObject* obj)
	{
		if (!obj) return;

		if (shadowShader != nullptr && obj->GetMesh() != nullptr)
		{
			shadowShader->SetMat4("model", obj->transform->GetWorldMatrix());
			obj->Draw();
		}

		for (GameObject* child : obj->childObjects)
		{
			RenderGameObjectShadowRecursive(child);
		}
	}

	Renderer::Renderer()
	{
		shadowShader = new Shader();
		shadowShader->LoadFromFile("Shaders/ShadowDepth.vert", "Shaders/ShadowDepth.frag");
		shadowMap.Create(1024, 1024);
	}

	void Renderer::Clear()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::Render(Scene& scene,ICamera* cam, bool isEngineCam)
	{
		RenderShadowPass(scene);
		RenderScenePass(scene, cam);
		Skybox* skybox = scene.GetSkybox();

		if (skybox != NULL && isEngineCam == true)
		{
			skybox->Draw(*cam);
		}
		if (isEngineCam == true)
		{
			DrawLightDebugGizmo(scene);
			DebugDraw::Draw(scene);
		}
	}

	void Renderer::RenderScenePass(Scene& scene, ICamera* cam)
	{
		std::vector<GameObject*>& gameobjects = scene.GetGameObjects();

		for (GameObject* obj : gameobjects)
		{
			if (obj)
			{
				RenderGameObjectRecursive(obj, scene, cam);
			}
		}
	}

	void Renderer::RenderShadowPass(Scene& scene)
	{
		DirectionalLight* dl = scene.GetDirectionalLight();
		if (dl != nullptr)
		{
			glm::vec3 sceneCenter(0.0f, 0.0f, 0.0f);
			glm::mat4 lightSpaceMatrix = dl->GetLightSpaceMatrix(sceneCenter);
			shadowMap.BindForWriting();
			shadowShader->Use();
			shadowShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
			std::vector<GameObject*>& gameobjects = scene.GetGameObjects();
			for (GameObject* obj : gameobjects)
			{
				if (obj)
				{
					RenderGameObjectShadowRecursive(obj);
				}
			}
			shadowMap.Unbind(scene.GetSceneBuffer()->GetWidth(), scene.GetSceneBuffer()->GetHeight());
		}
	}

	void Renderer::DrawLightDebugGizmo(Scene& scene)
	{
		DirectionalLight* dl = scene.GetDirectionalLight();
		if(dl != nullptr)
			scene.GetDirectionalLight()->Draw();

		std::vector<PointLight*> plLights = scene.GetPointLights();
		for (int i = 0; i < plLights.size(); i++)
		{
			PointLight* pl = plLights[i];
			if(pl != nullptr)
				pl->Draw();
		}

		std::vector<SpotLight*> slLights = scene.GetSpotLights();
		for (int i = 0; i < slLights.size(); i++)
		{
			SpotLight* sl = slLights[i];
			if(sl != nullptr)
				sl->Draw();
		}
	}
}