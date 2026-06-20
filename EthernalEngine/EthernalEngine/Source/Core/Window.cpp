#include "Core/Window.h"

#include "stb_image.h"
#include <iostream>

namespace EthernalEngine
{
	bool Window::CreateWindow(const char* title, int defaultWidth, int defaultHeight, bool shouldFullScreen = false)
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (shouldFullScreen) glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		m_window = glfwCreateWindow(defaultWidth, defaultHeight, title, nullptr, nullptr);
		m_width = defaultWidth;
		m_height = defaultHeight;

		if (m_window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);

		glfwSetWindowUserPointer(m_window, this);

		glfwGetFramebufferSize(m_window, &m_width, &m_height);

		SetIcon();

		glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

		return true;
	}

	void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);

		Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));

		if (windowInstance) {
			windowInstance->m_width = width;
			windowInstance->m_height = height;
		}
	}

	void Window::SetCursorPosCallback(GLFWcursorposfun callback)
	{
		glfwSetCursorPosCallback(m_window, callback);
	}

	void Window::SetScrollCallback(GLFWscrollfun callback)
	{
		glfwSetScrollCallback(m_window, callback);
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	void Window::SetIcon()
	{
		int width, height, channels;

		unsigned char* image = stbi_load("EngineResources/EthernalEngineIcon.png", &width, &height, &channels, 4);

		if (image)
		{
			GLFWimage images[1];
			images[0].width = width;
			images[0].height = height;
			images[0].pixels = image;

			glfwSetWindowIcon(m_window, 1, images);

			stbi_image_free(image);
		}
		else
		{
			std::cout << "Failed to load application Icon." << std::endl;

		}
	}

	Window::~Window()
	{
		if (m_window != nullptr)
		{
			glfwDestroyWindow(m_window);
		}

		glfwTerminate();
	}
}