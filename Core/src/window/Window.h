#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../CoreAPI.h"

namespace Window
{
	// Singleton Window class which ensures just one window instance at all times
	class CORE_API Window
	{
	private:
		static Window m_Instance;
		GLFWwindow* m_Window;
		int m_Width;
		int m_Height;

		Window() = default;
	public:
		Window(const Window& obj) = delete;
		~Window();

		static Window& Get() { return m_Instance; }
		static GLFWwindow* GetObjPtr() { return m_Instance.m_Window; }
		static int GetWidth() { return m_Instance.m_Width; }
		static int GetHeight() { return m_Instance.m_Height; }

		/**
		 * \brief Initializes GLFW, GLAD and creates the window
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
	};
}