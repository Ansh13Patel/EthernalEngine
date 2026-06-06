#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace EthernalEngine
{
    class Window
    {
    public:
        Window() = default;

        bool CreateWindow(const char* title, int defaultWidth, int defaultHeight, bool shouldFullScreen);

        void SetCursorPosCallback(GLFWcursorposfun callback);

        void SetScrollCallback(GLFWscrollfun callback);

        void SwapBuffers();

        bool WindowShouldClose()
        {
            return glfwWindowShouldClose(m_window);
        }

        GLFWwindow* GetGLFWwindow()
        {
            return m_window;
        }

        ~Window();

    private:

        static void FramebufferSizeCallback(
            GLFWwindow* window,
            int width,
            int height
        );

        GLFWwindow* m_window = nullptr;
    };
}