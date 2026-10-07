#pragma once

#include "Rendering/Mesh.h"

namespace EthernalEngine
{
	class PlaneMesh : public Mesh
	{
	public:
		PlaneMesh(float width, float height, float thickness);
		~PlaneMesh() = default;

	private:
		std::vector<Vertex> GenerateVertices(float width, float height, float thickness);
		std::vector<unsigned int> GenerateIndices();
	};
}
