#include "Input.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <iostream>

namespace EthernalEngine
{
	bool Input::isMouseLeftButtonDown = false;
	bool Input::isMouseRightButtonDown = false;

	float Input::lastX = 400.0f;
	float Input::lastY = 300.0f;

	bool Input::firstMouse = true;

	Camera* Input::camera = nullptr;
	Window* Input::window = nullptr;


	Input::Input(Window* window, Camera* camera)
	{
		Input::window = window;
		Input::camera = camera;

		//glfwSetCursorPosCallback(Input::window->GetGLFWwindow(), Mouse_Callback);

		glfwSetScrollCallback(Input::window->GetGLFWwindow(), Scroll_Callback);
	}

	void Input::ProcessKeyAndMouseInput(float deltatime)
	{
		if (window)
		{
			bool forward = false, backward = false, right = false, left = false;
			GLFWwindow* glfwWindow = window->GetGLFWwindow();

			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureKeyboard || io.WantCaptureMouse)
			{
				isMouseLeftButtonDown = false;
				isMouseRightButtonDown = false;
				firstMouse = true;
				return;
			}

			if (glfwGetKey(glfwWindow, GLFW_KEY_W)) forward = true;
			if (glfwGetKey(glfwWindow, GLFW_KEY_S)) backward = true;
			if (glfwGetKey(glfwWindow, GLFW_KEY_A)) left = true;
			if (glfwGetKey(glfwWindow, GLFW_KEY_D)) right = true;

			bool previousMouseState = isMouseLeftButtonDown;
		    isMouseLeftButtonDown = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
			isMouseRightButtonDown = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

			camera->MoveCamera(forward, backward, right, left, deltatime);

			if (isMouseLeftButtonDown && !previousMouseState) firstMouse = true;
			if (!isMouseLeftButtonDown) firstMouse = false;

			double xpos = 0, ypos = 0;
			glfwGetCursorPos(glfwWindow, &xpos, &ypos);
			Mouse_Callback(xpos, ypos);
		}
	}

	void Input::Mouse_Callback(double xpos, double ypos)
	{
		if (isMouseLeftButtonDown == false)
		{
			return;
		}

		if (firstMouse)
		{
			lastX = static_cast<float>(xpos);

			lastY = static_cast<float>(ypos);

			firstMouse = false;
		}

		float xoffset = static_cast<float>(xpos) - lastX;

		float yoffset = lastY - static_cast<float>(ypos);

		lastX = static_cast<float>(xpos);

		lastY = static_cast<float>(ypos);

		float sensitivity = 0.1f;

		xoffset *= sensitivity;

		yoffset *= sensitivity;

		if (camera)
		{
			camera->UpdateCameraRotation(xoffset, yoffset);
		}
	}

	void Input::Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (camera)
		{
			camera->UpdateCameraFov(static_cast<float>(yoffset));
		}
	}
}