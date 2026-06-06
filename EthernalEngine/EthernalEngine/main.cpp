#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "EditorUI.h"

#include <glad/glad.h>
#include <iostream>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

bool SetupAndInitialize(EthernalEngine::Window& window, EthernalEngine::EditorUI editorUI, bool shouldFullScreen);

int main()
{
	EthernalEngine::EditorUI editorUI;
    EthernalEngine::Window window;
	EthernalEngine::Renderer renderer;
	EthernalEngine::Scene scene;

    if (!SetupAndInitialize(window, editorUI, true))
    {
        std::cout << "Failed to setup and initialize" << std::endl;
        return -1;
    }

    float deltaFrame = 0.0f;
    float lastFrame = 0.0f;

    while (!window.WindowShouldClose())
    {
        editorUI.BeginFrame();
		editorUI.RenderUI(&scene);

        float currentFrame = glfwGetTime();
        deltaFrame = currentFrame - lastFrame;
        lastFrame = currentFrame;

		renderer.Clear();

        scene.Update(deltaFrame);
        renderer.Draw(scene.GetGameObjects(), &scene.GetCamera());

        std::cout << scene.GetGameObjectCount() << std::endl;

		editorUI.EndFrame();    
        glfwPollEvents();
        window.SwapBuffers();
    }

	editorUI.Shutdown();
    return 0;
}

bool SetupAndInitialize(EthernalEngine::Window& window, EthernalEngine::EditorUI editorUI, bool shouldFullScreen = false)
{
    if (!window.CreateWindow("Ethernal Engine", WIDTH, HEIGHT, shouldFullScreen))
    {
        std::cout<< "Failed to create window"<< std::endl;
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<< "Failed to initialize GLAD"<< std::endl;
        return false;
    }

	editorUI.Initialize(window.GetGLFWwindow());

    glEnable(GL_DEPTH_TEST);

    return true;
}