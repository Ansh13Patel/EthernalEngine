#pragma once

#include "Mesh.h"
#include "Texture.h"
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
