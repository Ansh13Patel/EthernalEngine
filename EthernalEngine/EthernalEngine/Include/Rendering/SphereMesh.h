#pragma once

#include "Rendering/Mesh.h"

namespace EthernalEngine
{
	class SphereMesh : public Mesh
	{
	public:
		SphereMesh(float radius, unsigned int segments, unsigned int rings);
		~SphereMesh() = default;
	};
}