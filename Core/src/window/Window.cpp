#include <iostream>

#include "Window.h"
#include "../utils/Logger.h"

namespace Window
{
	Window Window::m_Instance;

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::Init(int width, int height, const char* title)
	{
		if (!glfwInit())
		{
			LOG_ERROR("Failed to initialize GLFW!")
			return;
		}

		LOG_INFO("Initialized GLFW " <<  glfwGetVersionString())

		m_Width = width;
		m_Height = height;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);

		if (m_Window == nullptr)
		{
			LOG_ERROR("Failed to create GLFW window!")
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Failed to initialize GLAD!")
		}

		LOG_INFO("OpenGL " << glGetString(GL_VERSION))
	}

	int Window::ShouldClose()
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::PollAndSwapBuffers()
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}
