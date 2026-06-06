#pragma once

#include "Mesh.h"

namespace EthernalEngine
{
	class CubeMesh : public Mesh
	{
	public:
		CubeMesh();
		~CubeMesh() override = default;
		void Draw() override;
	};
}
