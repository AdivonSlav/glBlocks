#include <iostream>

#include "Window.h"
#include "../utils/Logger.h"
#include "../utils/GLErrorCallback.h"

namespace CoreWindow
{
	Window Window::m_Instance;
	bool Window::m_Keys[GLFW_KEY_LAST] = { false };

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

		// Sets the OpenGL version that will be used (4.6) and selects the core profile (without deprecated features)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// To maintain compatibility with MacOS systems
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		// For debug logging
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

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

		// Enables OpenGL debug output and sets the callback function for any errors so they can be logged to the console
		glfwSetErrorCallback(GLFWErrorCallback);
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(ErrorMessageCallback, nullptr);

		glfwSetKeyCallback(m_Window, KeyCallback);
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

	void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		m_Keys[key] = action != GLFW_RELEASE;
	}

	bool Window::IsKeyPressed(unsigned int key)
	{
		if (key >= GLFW_KEY_LAST)
			return false;

		return m_Keys[key];
	}

	bool Window::IsKeyReleased(unsigned int key)
	{
		if (key >= GLFW_KEY_LAST)
			return false;

		return !m_Keys[key];
	}
}
