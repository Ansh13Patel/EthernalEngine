#include "Rendering/Model.h"
#include "Scene/GameObject.h"

#include <iostream>

namespace EthernalEngine
{
	Model::Model(const std::string& path, GameObject* parent)
	{
		LoadModel(path, parent);
	}

	void Model::Draw()
	{
		for (auto& mesh : meshes)
		{
			mesh->Draw();
		}
	}

	void Model::LoadModel(const std::string& path, GameObject* parent)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals 
			| aiProcess_JoinIdenticalVertices);
		
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << importer.GetErrorString() << std::endl;
			return;
		}

		ProcessRootNode(scene->mRootNode, scene, parent);
	}

	std::unique_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::string texturePath;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
		{
			Vertex vertex;

			vertex.position = {
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			};

			vertex.normal = {
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			};

			if (mesh->mTextureCoords[0])
			{
				vertex.texCoord = {
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				};
			}
			else
			{
				vertex.texCoord = {
					0.0f,
					0.0f
				};
			}

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		std::shared_ptr<Texture> texture;
		texture = std::make_shared<Texture>();

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiString path;
			if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == AI_SUCCESS) 
			{
				texturePath = path.C_Str();

				const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(texturePath.c_str());
				if (embeddedTexture)
				{
					texture->LoadTextureFromMemory(reinterpret_cast<unsigned char*>(embeddedTexture->pcData), 
						embeddedTexture->mWidth);
				}
				else
				{
					texture->LoadTextureFromPath(texturePath.c_str());
				}
			}
		}

		std::unique_ptr<Mesh> meshobj = std::make_unique<Mesh>(vertices, indices, texture);

		GameObject* childMesh = new GameObject(mesh->mName.C_Str());
		childMesh->SetMesh(meshobj.get());
		parent->childObjects.push_back(childMesh);
	
		return meshobj;
	}

	void Model::ProcessRootNode(aiNode* node, const aiScene* scene, GameObject* parent)
	{
		parent->name = node->mName.C_Str();
		SetTransform(parent, node);

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene, parent));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, parent);
		}
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, GameObject* parent)
	{
		GameObject* childNode = new GameObject(node->mName.C_Str());
		SetTransform(childNode, node);
		parent->childObjects.push_back(childNode);

		for (unsigned int i = 0; i < node->mNumMeshes; i++) 
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene, childNode));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) 
		{
			ProcessNode(node->mChildren[i], scene, childNode);
		}
	}

	void Model::SetTransform(GameObject* obj, aiNode* node)
	{
		aiVector3D position, scale;
		aiQuaternion rotation;

		node->mTransformation.Decompose(scale, rotation, position);

		obj->transform.position =
		{
			position.x,
			position.y,
			position.z
		};

		obj->transform.scale =
		{
			scale.x,
			scale.y,
			scale.z
		};

		glm::quat rotQuat(rotation.w, rotation.x, rotation.y, rotation.z);

		obj->transform.rotation = glm::degrees(glm::eulerAngles(rotQuat));
	}
}