#include "Scene/GameObject.h"

namespace EthernalEngine
{
	void GameObject::Draw()
	{
        if (mesh && material)
        {
            mesh->Draw();
        }
	}

    void GameObject::SetMesh(Mesh* newMesh)
    {
        mesh = newMesh;
    }

    void GameObject::SetMaterial(Material* newMaterial)
    {
		material = newMaterial;
    }

    void GameObject::AddComponent(Component* component)
    {
        if (component != nullptr)
        {
            components.push_back(component);
        }
	}

    template<typename T>
    T* GameObject::GetComponent()
    {
        for (Component* component : components)
        {
            T* castedComponent = dynamic_cast<T*>(component);
            if (castedComponent != nullptr)
            {
                return castedComponent;
            }
        }
        return nullptr;
    }

    void GameObject::SetParentObject(GameObject* newParent) 
    {
        parent = newParent; 
        transform->SetParent(newParent ? newParent->transform : nullptr);
    }

    void GameObject::AddChildObject(GameObject* child)
    {
        if (child != nullptr)
        {
			child->SetParentObject(this);
            childObjects.push_back(child);
        }
    }
}