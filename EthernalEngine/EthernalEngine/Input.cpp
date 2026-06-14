#include "Input.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "ImGuizmo.h"

#include <iostream>

namespace EthernalEngine
{
	bool Input::isMouseLeftButtonDown = false;
	bool Input::isMouseRightButtonDown = false;
	bool Input::canSelectGameobject = false;

	float Input::lastX = 400.0f;
	float Input::lastY = 300.0f;

	bool Input::firstMouse = true;

	Camera* Input::camera = nullptr;
	Window* Input::window = nullptr;
	Scene* Input::scene = nullptr;


	Input::Input(Window* window, Scene* scene)
	{
		Input::window = window;
		Input::scene = scene;
		Input::camera = &(scene->GetCamera());

		glfwSetScrollCallback(Input::window->GetGLFWwindow(), Scroll_Callback);
	}

	void Input::ProcessKeyAndMouseInput(float deltatime)
	{
		if (window)
		{
			bool forward = false, backward = false, right = false, left = false;
			GLFWwindow* glfwWindow = window->GetGLFWwindow();

			ImGuiIO& io = ImGui::GetIO();
			if (io.WantCaptureKeyboard || (io.WantCaptureMouse && !ImGuizmo::IsOver()))
			{
				isMouseLeftButtonDown = false;
				isMouseRightButtonDown = false;
				firstMouse = true;
				return;
			}

			bool previousMouseRightState = isMouseRightButtonDown;
			bool previousMouseLeftState = isMouseLeftButtonDown;
		    isMouseLeftButtonDown = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
			isMouseRightButtonDown = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

			if (isMouseLeftButtonDown || previousMouseLeftState)
			{
				if (glfwGetKey(glfwWindow, GLFW_KEY_W)) forward = true;
				if (glfwGetKey(glfwWindow, GLFW_KEY_S)) backward = true;
				if (glfwGetKey(glfwWindow, GLFW_KEY_A)) left = true;
				if (glfwGetKey(glfwWindow, GLFW_KEY_D)) right = true;
			}

			camera->MoveCamera(forward, backward, right, left, deltatime);

			if (isMouseRightButtonDown && !previousMouseRightState) firstMouse = true;
			if (!isMouseRightButtonDown) firstMouse = true;

			if(isMouseLeftButtonDown && !previousMouseLeftState) canSelectGameobject = true;
			if (!isMouseLeftButtonDown) canSelectGameobject = false;

			double xpos = 0, ypos = 0;
			glfwGetCursorPos(glfwWindow, &xpos, &ypos);
			GameObjectSelection(window->GetWidth(), window->GetHeight(), xpos, ypos);
			Mouse_Callback(xpos, ypos);
		}
	}

	void Input::GameObjectSelection(int width, int height, double xpos, double ypos)
	{
		if (!canSelectGameobject) return;
		canSelectGameobject = false;

		float x = (2.0f * xpos) / width - 1.0f;
		float y = 1.0f - (2.0f * ypos) / height;
		glm::vec4 rayclip = { x,y,-1.0f,1.0f };
		glm::vec4 rayEye = glm::inverse(camera->GetProjectionMatrix()) * rayclip;
		rayEye = { rayEye.x, rayEye.y, -1.0f, 0.0f };
		glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(camera->GetViewMatrix()) * rayEye));
		scene->SelectGameObject(rayWorld);
	}

	void Input::Mouse_Callback(double xpos, double ypos)
	{
		if (isMouseRightButtonDown == false)
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