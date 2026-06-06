#pragma once

#include "GameObject.h"
#include "Camera.h"
#include <vector>

namespace EthernalEngine
{
	class Renderer 
	{
	public:
		Renderer() = default;
		~Renderer() = default;
		void Clear();
		void Draw(const std::vector<GameObject*>& gameobjects, const Camera* camera);
	};
}