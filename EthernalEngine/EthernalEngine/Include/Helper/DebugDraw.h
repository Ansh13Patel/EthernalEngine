#pragma once

#include "Rendering/Shader.h"
#include "Scene/Scene.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <assimp/texture.h>

#include <vector>
#include <memory>
#include <string>


namespace EthernalEngine
{
	struct DebugVertex
	{
		float position[3];
		float color[4];

		DebugVertex(const glm::vec3& pos, const glm::vec4& lineColor)
		{
			position[0] = pos.x;
			position[1] = pos.y;
			position[2] = pos.z;

			color[0] = lineColor.r;
			color[1] = lineColor.g;
			color[2] = lineColor.b;
			color[3] = lineColor.a;
		}
	};
	enum CirclePlane
	{
		XY,
		XZ,
		YZ
	};

	class DebugDraw
	{
	public:
		DebugDraw() = default;
		~DebugDraw() = default;
		static void Init();
        static void DrawLine(glm::vec3 startPos, glm::vec3 endpos, glm::vec4 color);
		static void DrawCircle(glm:: vec3 centerPos, float radius, glm::vec4 color, CirclePlane plane = CirclePlane::XY);
		static void DrawSphere(glm::vec3 centerPos, float radius, glm::vec4 color);
		static void DrawCone(glm::vec3 tipPos, glm::vec3 forwardDir, float angle, float height, glm::vec4 color);
		static void Draw(Scene& scene);
		static void Clear();

	private:
		static std::vector<DebugVertex> vertices;
		static unsigned int VAO;
		static unsigned int VBO;
		static Shader* shader;
		static unsigned int segments;
	};
}