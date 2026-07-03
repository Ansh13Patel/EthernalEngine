#include "Rendering/Model.h"
#include "Scene/GameObject.h"

#include <iostream>

namespace EthernalEngine
{
	Model::Model(const std::string& path, GameObject* parent, Shader* defaultShader)
	{
		LoadModel(path, parent, defaultShader);
	}

	void Model::LoadModel(const std::string& path, GameObject* parent, Shader* defaultShader)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals
			| aiProcess_JoinIdenticalVertices);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << importer.GetErrorString() << std::endl;
			return;
		}

		ProcessRootNode(scene->mRootNode, scene, parent, defaultShader);
	}

	std::unique_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* obj, Shader* defaultShader)
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
			else
			{
				texture->LoadTextureFromPath("Textures/White.png");
			}
		}

		std::unique_ptr<Mesh> meshobj = std::make_unique<Mesh>(vertices, indices);
		Material* material = new Material();
		material->SetBaseTexture(texture);
		material->SetShader(defaultShader);

		obj->SetMesh(meshobj.get());
		obj->SetMaterial(material);

		return meshobj;
	}

	void Model::ProcessRootNode(aiNode* node, const aiScene* scene, GameObject* parent, Shader* defaultShader)
	{
		parent->name = node->mName.C_Str();
		SetTransform(parent, node);

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene, parent, defaultShader));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, parent, defaultShader);
		}
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, GameObject* parent, Shader* defaultShader)
	{
		GameObject* childNode = new GameObject(node->mName.C_Str());
		SetTransform(childNode, node);
		parent->AddChildObject(childNode);

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene, childNode, defaultShader));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, childNode, defaultShader);
		}
	}

	void Model::SetTransform(GameObject* obj, aiNode* node)
	{
		aiVector3D position, scale;
		aiQuaternion rotation;

		node->mTransformation.Decompose(scale, rotation, position);

		obj->transform->position =
		{
			position.x,
			position.y,
			position.z
		};

		obj->transform->scale =
		{
			scale.x,
			scale.y,
			scale.z
		};

		glm::quat rotQuat(rotation.w, rotation.x, rotation.y, rotation.z);

		obj->transform->rotation = rotQuat;
	}
}