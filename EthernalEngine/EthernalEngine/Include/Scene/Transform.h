#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

namespace EthernalEngine
{
	class Transform
	{
	public:
		Transform();
		~Transform() = default;
        glm::vec3 GetForward() const;
		glm::vec3 GetRight() const;
		glm::vec3 GetUp() const;
		glm::mat4 GetLocalMatrix();
		glm::mat4 GetWorldMatrix();
		void SetParent(Transform* newParent) { parent = newParent; }

	public:
		Transform* parent = nullptr;
		glm::vec3 position = glm::vec3(0.0f);
		glm::quat rotation = glm::quat(glm::vec3(0.0f));
		glm::vec3 scale = glm::vec3(1.0f);
	};
}