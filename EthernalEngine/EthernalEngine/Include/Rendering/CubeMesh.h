#pragma once

#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"
#include <memory>

namespace EthernalEngine
{
	class CubeMesh : public Mesh
	{
	public:
		CubeMesh();
		~CubeMesh() = default;
	};
}
