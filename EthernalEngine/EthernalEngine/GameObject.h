#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

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

        void SetTexture(Texture* newTexture);

        Shader* GetShader() { return shader; }

        Mesh* GetMesh() { return mesh; }

        Texture* GetTexture() { return texture; }

        virtual void Update(
            float deltaTime
        ){  }

        virtual void Draw();

        Transform transform;

    private:

        std::string name;
        Mesh* mesh = nullptr;
		Shader* shader = nullptr;
		Texture* texture = nullptr;
    };
}