#pragma once
#include "Mesh.h"

namespace EthernalEngine
{
	class CylinderMesh : public Mesh
	{
	public:
		CylinderMesh(float radius = 0.5f, float height = 1.0f, int segments = 32);
		~CylinderMesh() = default;
	};
}