#include "GameObject.h"

namespace EthernalEngine
{
	void GameObject::Draw()
	{
        if ((!mesh && !model) || !shader)
            return;

        shader->SetMat4("model", transform.GetModelMatrix());

        shader->SetFloat3("colorMultiplier", color);

        if (model) 
        {
            model->Draw();
        }
        else if (mesh)
        {
            mesh->Draw();
        }
	}

    void GameObject::SetMesh(Mesh* newMesh)
    {
        mesh = newMesh;
    }

    void GameObject::SetModel(Model* newModel)
    {
        model = newModel;
    }

    void GameObject::SetShader(Shader* newShader)
    {
        shader = newShader;
    }

}