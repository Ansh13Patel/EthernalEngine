#include "Helper/DebugDraw.h"

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

	void DebugDraw::DrawCircle(glm::vec3 centerPos, float radius, glm::vec4 color, bool xy, bool xz, bool yz)
	{
		for (int i = 0; i < segments; i++)
		{
			float a0 = (float)i / segments * glm::two_pi<float>();
			float a1 = (float)(i + 1) / segments * glm::two_pi<float>();

			glm::vec3 p0 = glm::vec3(0.0f);
			glm::vec3 p1 = glm::vec3(0.0f);

			if (xy)
			{
				p0 = centerPos + glm::vec3(cos(a0), sin(a0), 0) * radius;
				p1 = centerPos + glm::vec3(cos(a1), sin(a1), 0) * radius;
			}
			else if (xz)
			{
				p0 = centerPos + glm::vec3(cos(a0), 0, sin(a0)) * radius;
				p1 = centerPos + glm::vec3(cos(a1), 0, sin(a1)) * radius;
			}
			else if (yz)
			{
				p0 = centerPos + glm::vec3(0, sin(a0), cos(a0)) * radius;
				p1 = centerPos + glm::vec3(0, sin(a1), cos(a1)) * radius;
			}
			
			DrawLine(p0, p1, color);
		}
	}

	void DebugDraw::DrawSphere(glm::vec3 centerPos, float radius, glm::vec4 color)
	{
		DrawCircle(centerPos, radius, color, true, false, false);
		DrawCircle(centerPos, radius, color, false, true, false);
		DrawCircle(centerPos, radius, color, false, false, true);
	}

	void DebugDraw::DrawCone()
	{

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