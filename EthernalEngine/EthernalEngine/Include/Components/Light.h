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
		json SerializeComponent() const override
		{
			json componentJson;
			componentJson["color"] = { lightColor[0], lightColor[1], lightColor[2] };
			return componentJson;
		}
		void DeserializeComponent(const json& componentJson) override
		{
			if (componentJson.contains("color") && componentJson["color"].is_array() && componentJson["color"].size() == 3)
			{
				lightColor[0] = componentJson["color"][0].get<float>();
				lightColor[1] = componentJson["color"][1].get<float>();
				lightColor[2] = componentJson["color"][2].get<float>();
			}
		}

	public:
		float lightColor[4]{ 1.0f,1.0f,1.0f,1.0f };
		float intensity;
		float specularStrength;
		bool enable;
	};
}
