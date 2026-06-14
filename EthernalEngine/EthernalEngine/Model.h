#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace EthernalEngine
{
	class Model
	{
	public:
		Model(const std::string& path);
		~Model() = delete;

		void Draw();
		
	private: 
		std::vector<std::unique_ptr<Mesh>> meshes;

	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
        std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};
}
