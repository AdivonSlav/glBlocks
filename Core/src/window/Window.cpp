#include "PCH.h"

#include "Window.h"
#include "../utils/Logger.h"
#include "../utils/GLErrorCallback.h"
#include "../utils/Dashboard.h"

namespace CoreWindow
{
	Window *Window::m_Instance = new Window;
	bool Window::m_Keys[GLFW_KEY_LAST] = {false};
	bool Window::m_MouseButtons[GLFW_MOUSE_BUTTON_LAST] = {false};
	bool Window::m_FirstClick = true;

	void Window::Init(int width, int height, const char *title)
	{
		LOG_INFO("Initializing GLFW...");
		if (!glfwInit())
		{
			LOG_ERROR("Failed to initialize GLFW!")
			return;
		}

		LOG_INFO("Initialized GLFW " << glfwGetVersionString())

		m_Width = width;
		m_Height = height;

		// Sets the OpenGL version that will be used and selects the core profile (without deprecated features)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// To maintain compatibility with MacOS systems
		// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		// Temporarily disables resizing of the window
		// glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		// For debug logging
#ifdef BLOCKS_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

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
#ifdef BLOCKS_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glfwSetErrorCallback(CoreUtils::GLFWErrorCallback);
		glDebugMessageCallback(CoreUtils::ErrorMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

		// Sets the corresponding callback methods
		glfwSetKeyCallback(m_Window, KeyCallback);
		glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
		glfwSetWindowSizeCallback(m_Window, ResizeCallback);

		// Centers window on screen
		const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(m_Window, mode->width / 2.0f - width / 2.0f, mode->height / 2.0f - height / 2.0f);
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

	void Window::AddResizeCallback(std::function<void(int, int)> callback)
	{
		m_CustomCallbacks.ResizeCallbacks.push_back(callback);
	}

	void Window::Cleanup()
	{
		delete m_Instance;
		LOG_INFO("Terminating GLFW...");
		glfwTerminate();
	}

	void Window::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action != GLFW_RELEASE)
			CoreUtils::Dashboard::SetShown(!CoreUtils::Dashboard::IsShown());

		// If the key has been pressed, then the corresponding array member is set to true
		m_Keys[key] = action != GLFW_RELEASE;
	}

	void Window::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
	{
		m_MouseButtons[button] = action != GLFW_RELEASE;
	}

	void Window::ResizeCallback(GLFWwindow *window, int width, int height)
	{
		m_Instance->m_Width = width;
		m_Instance->m_Height = height;

		glViewport(0, 0, width, height);

		for (auto &callback : m_Instance->m_CustomCallbacks.ResizeCallbacks)
		{
			callback(width, height);
		}
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

	void Window::SetCursorPos(const glm::vec2 &position)
	{
		glfwSetCursorPos(m_Instance->m_Window, position.x, position.y);
	}

	void Window::SetCursorMode(CursorMode mode)
	{
		switch (mode)
		{
		case CursorMode::NORMAL:
			glfwSetInputMode(m_Instance->m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		case CursorMode::HIDDEN:
			glfwSetInputMode(m_Instance->m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			break;
		case CursorMode::DISABLED:
			glfwSetInputMode(m_Instance->m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		}
	}

	glm::vec2 Window::GetCursorPos()
	{
		double xpos, ypos;
		glfwGetCursorPos(m_Instance->m_Window, &xpos, &ypos);

		return {xpos, ypos};
	}
}
