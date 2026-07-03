#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace EthernalEngine
{
	class GameObject;

	class Model
	{
	public:
		Model(const std::string& path, GameObject* parent,Shader* defaultShader);
		~Model() = delete;

	private: 
		std::vector<std::unique_ptr<Mesh>> meshes;

	private:
		void LoadModel(const std::string& path, GameObject* parent, Shader* defaultShader);
		void ProcessRootNode(aiNode* node, const aiScene* scene, GameObject* parent, Shader* defaultShader);
		void ProcessNode(aiNode* node, const aiScene* scene, GameObject* parent,Shader* defaultShader);
        std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent, Shader* defaultShader);
		void SetTransform(GameObject* obj, aiNode* node);
	};
}
