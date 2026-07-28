#pragma once

#include "Core/Window.h"
#include "Core/EngineCamera.h"
#include "Scene/Scene.h"
#include <Editor/EditorUI.h>

namespace EthernalEngine
{
	class Input
	{
	public:

		Input(Window* window, Scene* scene);

		~Input() = default;

		void ProcessKeyAndMouseInput(float deltatime, Viewport& viewport);

	private:

		void GameObjectSelection(int width, int height, double xpos, double ypos);
		static void Mouse_Callback(double xpos, double ypos);
		static void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);

	private:

		static bool isMouseLeftButtonDown;
		static bool isMouseRightButtonDown;
		static bool isMouseMiddleButtonDown;
		static bool canSelectGameobject;

		static float lastX;
		static float lastY;

		static bool firstMouse;
		static bool canMoveCamera;

		static bool viewportHovered;
		static bool viewportFocused;

		static EngineCamera* EngineCamera;
		static Window* window;
		static Scene* scene;
	};
}