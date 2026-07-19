#pragma once

#include "Scene/Transform.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "Components/Component.h"

#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace EthernalEngine
{
    class GameObject
    {
    public:

        GameObject(std::string name) : name(std::move(name)) { transform = new Transform(); }

        virtual ~GameObject();

        void SetMesh(Mesh* newMesh);

        void SetMaterial(Material* newMaterial);

        Mesh* GetMesh() { return mesh; }

        Material* GetMaterial() { return material; }

        void SetIsSelected(bool selected) { isSelected = selected; }

        bool GetIsSelected() { return isSelected; }

        unsigned int GetChildObjectCount() { return childObjects.size(); }

		std::vector<GameObject*> GetChildObjects() { return childObjects; }

		GameObject* GetParentObject() { return parent; }

        void SetParentObject(GameObject* newParent);

        void AddChildObject(GameObject* child);

        void AddComponent(Component* component);

		json SerializeGameObject() const;
		void DeserializeGameObject(const json& gameObjectJson);

        template<typename T>
        T* GetComponent();
        
        virtual void Update(
            float deltaTime
        ){  }

        virtual void Draw();

        Transform* transform;
        GameObject* parent = nullptr;
        std::vector<GameObject*> childObjects;
        std::string name;
        std::vector<Component*> components;
        std::string modelPath = "";
		DefaultMeshType defaultMeshType = DefaultMeshType::None;

    private:

        Mesh* mesh = nullptr;
		Material* material = nullptr;
        bool isSelected = false;
    };
}