#pragma once

#include "Scene/Transform.h"
#include <Components/Component.h>


namespace EthernalEngine
{
	class Light : public Component
	{
	public:
		virtual ~Light() = default;
		virtual void Draw() = 0;

	public:
		float lightColor[4]{ 1.0f,1.0f,1.0f,1.0f };
		float intensity;
		float specularStrength;
		bool enable;
	};
}
