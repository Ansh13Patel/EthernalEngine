#include "Helper/DebugDraw.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace EthernalEngine
{
	std::vector<DebugVertex> DebugDraw::vertices{};
	unsigned int DebugDraw::VAO = 0;
	unsigned int DebugDraw::VBO = 0;
	Shader* DebugDraw::shader = nullptr;
	unsigned int DebugDraw::segments = 64;

	void DebugDraw::Init()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));
		glEnableVertexAttribArray(1);

		if (shader == nullptr)
		{
			shader = new Shader();
			shader->LoadFromFile("Shaders/DebugShader.vert", "Shaders/DebugShader.frag");
		}
	}

    void DebugDraw::DrawLine(glm::vec3 startPos, glm::vec3 endpos, glm::vec4 color)
	{
		vertices.push_back(DebugVertex(startPos, color));
		vertices.push_back(DebugVertex(endpos, color));
	}

	void DebugDraw::DrawCircle(glm::vec3 centerPos, float radius, glm::vec4 color, CirclePlane plane)
	{
		for (int i = 0; i < segments; i++)
		{
			float a0 = (float)i / segments * glm::two_pi<float>();
			float a1 = (float)(i + 1) / segments * glm::two_pi<float>();

			glm::vec3 p0 = glm::vec3(0.0f);
			glm::vec3 p1 = glm::vec3(0.0f);

			switch (plane)
			{		
			case CirclePlane::XY:
				p0 = centerPos + glm::vec3(cos(a0), sin(a0), 0) * radius;
				p1 = centerPos + glm::vec3(cos(a1), sin(a1), 0) * radius;
				break;
			case CirclePlane::XZ:
				p0 = centerPos + glm::vec3(cos(a0), 0, sin(a0)) * radius;
				p1 = centerPos + glm::vec3(cos(a1), 0, sin(a1)) * radius;
				break;
			case CirclePlane::YZ:
				p0 = centerPos + glm::vec3(0, sin(a0), cos(a0)) * radius;
				p1 = centerPos + glm::vec3(0, sin(a1), cos(a1)) * radius;
				break;
			default:
				break;
			}
			
			DrawLine(p0, p1, color);
		}
	}

	void DebugDraw::DrawSphere(glm::vec3 centerPos, float radius, glm::vec4 color)
	{
		DrawCircle(centerPos, radius, color, CirclePlane::XY);
		DrawCircle(centerPos, radius, color, CirclePlane::XZ);
		DrawCircle(centerPos, radius, color, CirclePlane::YZ);
	}

	void DebugDraw::DrawCone(glm::vec3 tipPos, glm::vec3 forwardDir, float angle, float height, glm::vec4 color)
	{
		glm::vec3 baseCenter = tipPos + forwardDir * height;
		float radius = tan(glm::radians(angle * 0.5f)) * height;

		glm::quat rotation = glm::rotation(glm::vec3(0, 0, -1), forwardDir);

		for (int i = 0; i < segments; i++)
		{
			float a0 = i * glm::two_pi<float>() / segments;
			float a1 = (i + 1) * glm::two_pi<float>() / segments;

			glm::vec3 local0(cos(a0) * radius, sin(a0) * radius, 0.0f);

			glm::vec3 local1(cos(a1) * radius, sin(a1) * radius, 0.0f);

			glm::vec3 p0 = baseCenter + rotation * local0;
			glm::vec3 p1 = baseCenter + rotation * local1;

			DrawLine(p0, p1, color);
		}

		for (int i = 0; i < segments; i += 8)
		{
			float angle = i * glm::two_pi<float>() / segments;

			glm::vec3 local(cos(angle) * radius, sin(angle) * radius, 0);

			glm::vec3 edgePos = baseCenter + rotation * local;

			DrawLine(tipPos, edgePos, color);
		}
	}

	void DebugDraw::Clear()
	{
		vertices.clear();
	}

	void DebugDraw::Draw(Scene& scene)
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(DebugVertex), vertices.data(), GL_DYNAMIC_DRAW);
		
		shader->Use();
        shader->SetMat4("view", scene.GetCamera().GetViewMatrix());
		shader->SetMat4("projection", scene.GetCamera().GetProjectionMatrix());

		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.size()));
		
		glBindVertexArray(0);
		Clear();
	}
}