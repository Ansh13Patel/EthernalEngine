#pragma once

#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"
#include <memory>

namespace EthernalEngine
{
	class CubeMesh : public Mesh
	{
	public:
		CubeMesh(std::shared_ptr<Texture> texture);
		~CubeMesh() = default;
	};
}
