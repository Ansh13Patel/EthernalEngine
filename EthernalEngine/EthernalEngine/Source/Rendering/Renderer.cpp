#include "Rendering/Renderer.h"
#include "Scene/Scene.h"
#include "Helper/DebugDraw.h"

#include <iostream>

namespace EthernalEngine
{
	static void DrawGameObjectRecursive(GameObject* obj, Scene& scene, ICamera* cam)
	{
		if (!obj) return;

		Material* mat = obj->GetMaterial();
		if (mat != nullptr)
		{
			mat->Update(scene, obj, cam);
		}

		obj->Draw();

		for (GameObject* child : obj->childObjects)
		{
			DrawGameObjectRecursive(child, scene, cam);
		}
	}

	void Renderer::Clear()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::Draw(Scene& scene,ICamera* cam, bool isEngineCam)
	{
		std::vector<GameObject*>& gameobjects = scene.GetGameObjects();
		Skybox* skybox = scene.GetSkybox();

		for (GameObject* obj : gameobjects)
		{
			if (obj)
			{
				DrawGameObjectRecursive(obj, scene, cam);
			}
		}
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