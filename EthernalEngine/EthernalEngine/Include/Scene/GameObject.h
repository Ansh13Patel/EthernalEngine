#pragma once

#include "Scene/Transform.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "Components/Component.h"

#include <string>

namespace EthernalEngine
{
    class GameObject
    {
    public:

        GameObject(std::string name) : name(std::move(name)) { transform = new Transform(); }

        virtual ~GameObject() = default;

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

        template<typename T>
        T* GetComponent();
        
        virtual void Update(
            float deltaTime
        ){  }

        virtual void Draw();

        Transform* transform;
        GameObject* parent = nullptr;
        std::vector<GameObject*> childObjects;
        float color[3]{ 1.0f,1.0f,1.0f };
        std::string name;
        std::vector<Component*> components;

    private:

        Mesh* mesh = nullptr;
		Material* material = nullptr;
        bool isSelected = false;

    };
}