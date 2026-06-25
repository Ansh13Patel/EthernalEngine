#pragma once

#include "Scene/GameObject.h"
#include "Core/EngineCamera.h"

#include <vector>

namespace EthernalEngine
{
	class Scene;

	class Renderer 
	{
	public:
		Renderer() = default;
		~Renderer() = default;
		void Clear();
		void Draw(Scene* scene);
	};
}