#include "Scene/GameObject.h"
#include "Components/DirectionalLight.h"
#include "Components/PointLight.h"
#include "Components/SpotLight.h"

namespace EthernalEngine
{
    GameObject::~GameObject()
    {
        for (Component* c : components)
            delete c;
    }


	void GameObject::Draw()
	{
        if (mesh)
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

    json GameObject::SerializeGameObject() const
    {
        json gameObjectJson;
        gameObjectJson["name"] = name;
        gameObjectJson["position"] = { transform->position.x, transform->position.y, transform->position.z };
        gameObjectJson["rotation"] = { transform->rotation.x, transform->rotation.y, transform->rotation.z, transform->rotation.w };
        gameObjectJson["scale"] = { transform->scale.x, transform->scale.y, transform->scale.z };
		gameObjectJson["modelPath"] = modelPath;
		gameObjectJson["defaultMeshType"] = static_cast<int>(defaultMeshType);
        if (!components.empty())
        {
            gameObjectJson["components"] = json::array();
        }
        if (material != nullptr) 
        {
            gameObjectJson["material"] = material->SerializeMaterial();
        }

        for(const auto& component : components)
        {
            json componentJson = component->SerializeComponent();
            gameObjectJson["components"].push_back(componentJson);
		}
        return gameObjectJson;
	}

    void GameObject::DeserializeGameObject(const json& gameObjectJson)
    {
        if (gameObjectJson.contains("name"))
            name = gameObjectJson["name"].get<std::string>();
        if (gameObjectJson.contains("position"))
        {
            auto pos = gameObjectJson["position"];
            transform->position = glm::vec3(pos[0], pos[1], pos[2]);
        }
        if (gameObjectJson.contains("rotation"))
        {
            auto rot = gameObjectJson["rotation"];
            transform->rotation = glm::quat(rot[3], rot[0], rot[1], rot[2]);
        }
        if (gameObjectJson.contains("scale"))
        {
            auto scale = gameObjectJson["scale"];
            transform->scale = glm::vec3(scale[0], scale[1], scale[2]);
        }
        if (gameObjectJson.contains("modelPath"))
            modelPath = gameObjectJson["modelPath"].get<std::string>();
        if (gameObjectJson.contains("defaultMeshType"))
            defaultMeshType = static_cast<DefaultMeshType>(gameObjectJson["defaultMeshType"].get<int>());
        if (gameObjectJson.contains("components"))
        {
            for (const auto& componentJson : gameObjectJson["components"])
            {
                std::string type = componentJson["type"];
                Component* newComponent = nullptr;
                if (type == "DirectionalLight")
                    newComponent = new DirectionalLight(this);
                else if (type == "PointLight")
                    newComponent = new PointLight(this);
                else if (type == "SpotLight")
                    newComponent = new SpotLight(this);
                if (newComponent)
                {
                    newComponent->DeserializeComponent(componentJson);
                    AddComponent(newComponent);
                }
            }
        }
        if (gameObjectJson.contains("material"))
        {
            json materialJson = gameObjectJson["material"];
            material = new Material();
            material->DeserializeMaterial(materialJson);
        }
    }
}