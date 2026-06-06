#include "GameObject.h"

namespace EthernalEngine
{
	void GameObject::Draw()
	{
        if (!mesh || !shader)
            return;

        shader->Use();

        shader->SetMat4(
            "model",
            transform.GetModelMatrix()
        );

        if (texture)
        {
            texture->Bind();
        }

        mesh->Draw();
	}

    void GameObject::SetMesh(Mesh* newMesh)
    {
        mesh = newMesh;
    }

    void GameObject::SetShader(Shader* newShader)
    {
        shader = newShader;
    }

    void GameObject::SetTexture(Texture* newTexture)
    {
        texture = newTexture;
    }
}