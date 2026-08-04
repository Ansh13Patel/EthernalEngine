#pragma once

#include <glm/glm.hpp>

namespace EthernalEngine
{
	class ICamera
	{
	public:
        virtual glm::mat4 GetViewMatrix() const = 0;
		virtual glm::mat4 GetProjectionMatrix() const = 0;
		virtual glm::vec3 GetPosition() const = 0;
	};
}