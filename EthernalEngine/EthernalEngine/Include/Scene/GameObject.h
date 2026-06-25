#pragma once

#include "Scene/Transform.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/Model.h"
#include <Components/Component.h>

#include <string>

namespace EthernalEngine
{
    class GameObject
    {
    public:

        GameObject(std::string name) : name(std::move(name)) {}

        virtual ~GameObject() = default;

        void SetMesh(Mesh* newMesh);

        void SetShader(Shader* newShader);

        void SetModel(Model* newModel);

        Shader* GetShader() { return shader; }

        Mesh* GetMesh() { return mesh; }

        Model* GetModel() { return model; }

        unsigned int GetChildObjectCount() { return childObjects.size(); }

        virtual void Update(
            float deltaTime
        ){  }

        virtual void Draw();

        Transform transform;
        float color[3]{ 1.0f,1.0f,1.0f };
        std::string name;
        std::vector<GameObject*> childObjects;

        std::vector<Component*> components;

    private:

        Mesh* mesh = nullptr;
        Model* model = nullptr;
		Shader* shader = nullptr;

    };
}