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
	class GameObject;

	class Model
	{
	public:
		Model(const std::string& path, GameObject* parent);
		~Model() = delete;

		void Draw();
		
	private: 
		std::vector<std::unique_ptr<Mesh>> meshes;

	private:
		void LoadModel(const std::string& path, GameObject* parent);
		void ProcessRootNode(aiNode* node, const aiScene* scene, GameObject* parent);
		void ProcessNode(aiNode* node, const aiScene* scene, GameObject* parent);
        std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent);
		void SetTransform(GameObject* obj, aiNode* node);
	};
}
