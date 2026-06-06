#include "Window.h"

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

        if (m_window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(m_window);

        glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

        return true;
    }

    void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
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

    Window::~Window()
    {
        if (m_window != nullptr)
        {
            glfwDestroyWindow(m_window);
        }

        glfwTerminate();
    }
}