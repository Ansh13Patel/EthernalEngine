#pragma once

#include "Scene/GameObject.h"
#include "Scene/Camera.h"

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