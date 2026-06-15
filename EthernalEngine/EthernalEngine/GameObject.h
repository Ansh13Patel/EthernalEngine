#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"

#include<string>

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

        virtual void Update(
            float deltaTime
        ){  }

        virtual void Draw();

        Transform transform;
        float color[3]{ 1.0f,1.0f,1.0f };
        std::string name;

    private:

        Mesh* mesh = nullptr;
        Model* model = nullptr;
		Shader* shader = nullptr;
    };
}