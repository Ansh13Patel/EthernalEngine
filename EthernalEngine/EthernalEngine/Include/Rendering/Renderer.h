#pragma once

#include "Scene/GameObject.h"
#include "Core/EngineCamera.h"
#include "Rendering/ShadowMap.h"

#include <vector>

namespace EthernalEngine
{
	class Scene;

	class Renderer 
	{
	public:
		Renderer();
		~Renderer() = default;
		void Clear();
		void Render(Scene& scene, ICamera* cam, bool isEngineCam);

	private:
		void DrawLightDebugGizmo(Scene& scene);
		void RenderScenePass(Scene& scene,ICamera* cam);
		void RenderGameObjectRecursive(GameObject* obj, Scene& scene, ICamera* cam);
		void RenderShadowPass(Scene& scene);
		void RenderGameObjectShadowRecursive(GameObject* obj);

	private:
		ShadowMap& shadowMap = ShadowMap();
		Shader* shadowShader = nullptr;
	};
}