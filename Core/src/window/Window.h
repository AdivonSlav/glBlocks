#pragma once

#include "../CoreAPI.h"

namespace CoreWindow
{
	enum class CursorMode { NORMAL, HIDDEN, DISABLED };

	// Singleton Window class which ensures just one window instance at all times
	class CORE_API Window
	{
	private:
		static Window* m_Instance;
		GLFWwindow* m_Window;
		int m_Width;
		int m_Height;

		static bool m_Keys[GLFW_KEY_LAST];
		static bool m_MouseButtons[GLFW_MOUSE_BUTTON_LAST];

		static bool m_FirstClick;

		Window() = default;
	public:
		Window(const Window& obj) = delete;
		~Window() = default;

		/**
		 * \brief Initializes GLFW, GLAD and creates the window
		 * \param width Width of the window
		 * \param height Height of the window
		 * \param title Title of the window
		 */
		void Init(int width, int height, const char* title);

		/**
		 * \brief Notifies whether the window should be closed or not
		 * \return Value of the window close flag
		 */
		int ShouldClose();

		/**
		 * \brief Swaps the color buffer and polls for any events
		 */
		void PollAndSwapBuffers();

		/**
		 * \brief Terminates the GLFW application
		 */
		static void Cleanup();

		/**
		 * \brief Function that gets called any time a key has been pressed or released
		 */
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		/**
		 * \brief Function that gets called any time a mouse button has been pressed or released
		 */
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		/**
		 * \brief Function that gets called any time the mouse moves.
		 */
		static void CursorCallback(GLFWwindow* window, double xpos, double ypos);

		static bool IsKeyPressed(unsigned int key);
		static bool IsKeyReleased(unsigned int key);
		static bool IsMouseButtonPressed(unsigned int button);
		static bool IsMouseButtonReleased(unsigned int button);

		static Window& Get() { return *m_Instance; }
		static GLFWwindow* GetWindowPtr() { return m_Instance->m_Window; }
		static int GetWidth() { return m_Instance->m_Width; }
		static int GetHeight() { return m_Instance->m_Height; }

		static void SetCursorPos(const glm::vec2& position);
		static void SetCursorMode(CursorMode mode);
		static glm::vec2 GetCursorPos();

		/**
		 * \brief Returns the time elapsed since GLFW was initialized
		 */
		static double GetTime() { return glfwGetTime(); }
	};
}