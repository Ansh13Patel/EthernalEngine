#include "Core/Window.h"
#include "Rendering/Renderer.h"
#include "Scene/Scene.h"
#include "Editor/EditorUI.h"
#include "Core/Input.h"

#include <glad/glad.h>
#include <iostream>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

bool SetupAndInitialize(EthernalEngine::Window* window, EthernalEngine::EditorUI* editorUI, bool shouldFullScreen);

int main()
{
	EthernalEngine::EditorUI editorUI;
    EthernalEngine::Window window;
	EthernalEngine::Renderer renderer;

    if (!SetupAndInitialize(&window, &editorUI, true))
    {
        std::cout << "Failed to setup and initialize" << std::endl;
        return -1;
    }

    EthernalEngine::Scene scene{ &window, editorUI.viewport };
    EthernalEngine::Input input{ &window, &scene };

    float deltaFrame = 0.0f;
    float lastFrame = 0.0f;

    while (!window.WindowShouldClose())
    {
        glfwPollEvents();

        float currentFrame = glfwGetTime();
        deltaFrame = currentFrame - lastFrame;
        lastFrame = currentFrame;

        input.ProcessKeyAndMouseInput(deltaFrame, editorUI.viewport);

        editorUI.BeginFrame();
        editorUI.RenderUI(&scene);

        if (scene.GetSceneBuffer() != nullptr)
            scene.GetSceneBuffer()->Bind();

        renderer.Clear();
        scene.Update(deltaFrame);
        renderer.Draw(scene);

        if (scene.GetSceneBuffer() != nullptr)
            scene.GetSceneBuffer()->Unbind();

        editorUI.EndFrame();

        window.SwapBuffers();

        if (scene.pendingSceneLoad)
        {
            scene.LoadPendingScene();
        }
    }

	editorUI.Shutdown();
    return 0;
}

bool SetupAndInitialize(EthernalEngine::Window* window, EthernalEngine::EditorUI* editorUI, bool shouldFullScreen = false)
{
    if (!window->CreateWindow("Ethernal Engine", WIDTH, HEIGHT, shouldFullScreen))
    {
        std::cout<< "Failed to create window"<< std::endl;
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<< "Failed to initialize GLAD"<< std::endl;
        return false;
    }

	editorUI->Initialize(window->GetGLFWwindow());

    glEnable(GL_DEPTH_TEST);

    return true;
}