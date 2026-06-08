#pragma once

#include "Window.h"
#include "Camera.h"

namespace EthernalEngine
{
	class Input
	{
	public:

		Input(Window* window,Camera* camera);

		~Input() = default;

		void ProcessKeyAndMouseInput(float deltatime);

	private:

		static void Mouse_Callback(double xpos, double ypos);

		static void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);

	private:

		static bool isMouseLeftButtonDown;
		static bool isMouseRightButtonDown;

		static float lastX;
		static float lastY;

		static bool firstMouse;

		static Camera* camera;
		static Window* window;
	};
}