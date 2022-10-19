#include <iostream>

#include "Window.h"
#include "../utils/Logger.h"
#include "../utils/GLErrorCallback.h"

namespace CoreWindow
{
	Window Window::m_Instance;
	bool Window::m_Keys[GLFW_KEY_LAST] = { false };
	bool Window::m_MouseButtons[GLFW_MOUSE_BUTTON_LAST] = { false };

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
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

		// Temporarily disables resizing of the window
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glfwSetErrorCallback(GLFWErrorCallback);
		glDebugMessageCallback(ErrorMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		// Sets the corresponding callback methods for keyboard and mouse events
		glfwSetKeyCallback(m_Window, KeyCallback);
		glfwSetMouseButtonCallback(m_Window, MouseCallback);
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
		// If the key has been pressed, then the corresponding array member is set to true

		m_Keys[key] = action != GLFW_RELEASE;
	}

	void Window::MouseCallback(GLFWwindow* window, int button, int action, int mods)
	{
		m_MouseButtons[button] = action != GLFW_RELEASE;
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

	bool Window::IsMouseButtonPressed(unsigned int button)
	{
		if (button >= GLFW_MOUSE_BUTTON_LAST)
			return false;

		return m_MouseButtons[button];
	}

	bool Window::IsMouseButtonReleased(unsigned int button)
	{
		if (button >= GLFW_MOUSE_BUTTON_LAST)
			return false;

		return !m_MouseButtons[button];
	}

	void Window::GetCursorPos(double& x, double& y)
	{
		glfwGetCursorPos(m_Instance.m_Window, &x, &y);
	}
}
