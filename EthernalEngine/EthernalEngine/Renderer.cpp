#include "Renderer.h"

namespace EthernalEngine
{
	void Renderer::Clear() 
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::Draw(const std::vector<GameObject*>& gameobjects, const Camera* camera) 
	{
		Shader* currentShader = nullptr;
		for (GameObject* obj : gameobjects) 
		{
			if (obj) 
			{
				if (obj->GetShader() != currentShader && camera != nullptr)
				{
					obj->GetShader()->Use();
					obj->GetShader()->SetMat4("view", camera->GetViewMatrix());
					obj->GetShader()->SetMat4("projection", camera->GetProjectionMatrix());
					currentShader = obj->GetShader();
				}
				obj->Draw();
			}
		}
	}	
}