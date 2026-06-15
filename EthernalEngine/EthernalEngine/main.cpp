#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "EditorUI.h"
#include "Input.h"

#include <glad/glad.h>
#include <iostream>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

bool SetupAndInitialize(EthernalEngine::Window* window, EthernalEngine::EditorUI* editorUI, bool shouldFullScreen);

int main()
{
    std::cout << "C++ Version Value: " << __cplusplus << "\n";

    if (__cplusplus == 202302L) std::cout << "You are using C++23\n";
    else if (__cplusplus == 202002L) std::cout << "You are using C++20\n";
    else if (__cplusplus == 201703L) std::cout << "You are using C++17\n";
    else if (__cplusplus == 201402L) std::cout << "You are using C++14\n";
    else if (__cplusplus == 201103L) std::cout << "You are using C++11\n";
    else if (__cplusplus == 199711L) std::cout << "You are using C++98\n";
    else std::cout << "You are using a pre-standard or newer experimental version.\n";

	EthernalEngine::EditorUI editorUI;
    EthernalEngine::Window window;
	EthernalEngine::Renderer renderer;

    if (!SetupAndInitialize(&window, &editorUI, true))
    {
        std::cout << "Failed to setup and initialize" << std::endl;
        return -1;
    }

    EthernalEngine::Scene scene{ &window };
    EthernalEngine::Input input{ &window, &scene };

    float deltaFrame = 0.0f;
    float lastFrame = 0.0f;

    while (!window.WindowShouldClose())
    {
        glfwPollEvents();
        float currentFrame = glfwGetTime();
        deltaFrame = currentFrame - lastFrame;
        lastFrame = currentFrame;

        renderer.Clear();

        editorUI.BeginFrame();
		editorUI.RenderUI(&scene);

        input.ProcessKeyAndMouseInput(deltaFrame);

        scene.Update(deltaFrame);
        renderer.Draw(scene.GetGameObjects(), &scene.GetCamera());

		editorUI.EndFrame();    
        window.SwapBuffers();
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