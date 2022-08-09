#include "Camera.h"
#include "../utils/Logger.h"

using namespace CoreWindow;

namespace CoreGraphics
{
	Camera::Camera(glm::vec3 position)
		: m_Position(position), m_Orientation(0.0f, 0.0f, -1.0f), m_UpDir(0.0f, 1.0f, 0.0f), m_Sensitivity(6.0f), m_FirstClick(false)
	{

	}

	void Camera::SetMatrix(float fov, float nearPlane, float farPlane, Shader& shader, const char* uniform)
	{
		shader.Bind();

		// Initializing view and projection matrices as identity matrices
		auto view = glm::identity<glm::mat4>();
		auto projection = glm::identity<glm::mat4>();

		// Calculates the view and projection matrices
		view = glm::lookAt(m_Position, m_Position + m_Orientation, m_UpDir);
		projection = glm::perspective(glm::radians(fov), (float)(Window::GetWidth() / Window::GetHeight()), nearPlane, farPlane);

		// Sends the matrices to the uniform variable located in the vertex shader
		glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), uniform), 1, GL_FALSE, glm::value_ptr(projection * view));

		shader.Unbind();
	}

	void Camera::CheckInput(double deltaTime)
	{
		// To do: Implement delta time between frames in order to slow down the movements

		if (Window::IsKeyPressed(GLFW_KEY_W))
		{
			m_Position += (float)(deltaTime * m_Speed) * m_Orientation;
		}
		if (Window::IsKeyPressed(GLFW_KEY_A))
		{
			m_Position += (float)(deltaTime * m_Speed) * -glm::normalize(glm::cross(m_Orientation, m_UpDir));
		}
		if (Window::IsKeyPressed(GLFW_KEY_S))
		{
			m_Position += (float)(deltaTime * m_Speed) * -m_Orientation;
		}
		if (Window::IsKeyPressed(GLFW_KEY_D))
		{
			m_Position += (float)(deltaTime * m_Speed) * glm::normalize(glm::cross(m_Orientation, m_UpDir));
		}

		if (Window::IsKeyPressed(GLFW_KEY_SPACE))
		{
			m_Position += (float)(deltaTime * m_Speed) * m_UpDir;
		}
		if (Window::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			m_Position += (float)(deltaTime * m_Speed) * -m_UpDir;
		}

		if (Window::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			m_Speed = 1.0f;
		}
		if (Window::IsKeyReleased(GLFW_KEY_LEFT_SHIFT))
		{
			m_Speed = 0.5f;
		}

		if (Window::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			glfwSetInputMode(Window::GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			// If the first click is the current one, reset the mouse position to the center in order to prevent an accidental rotation
			if (m_FirstClick)
			{
				glfwSetCursorPos(Window::GetWindowPtr(), Window::GetWidth() / 2, Window::GetHeight() / 2);
				m_FirstClick = false;
			}

			double mouseX;
			double mouseY;
			Window::GetCursorPos(mouseX, mouseY);

			// Some mathematics mumbo jumbo

			float rotationX = m_Sensitivity * (float)(mouseY - (Window::GetHeight() / 2)) / Window::GetHeight();
			float rotationY = m_Sensitivity * (float)(mouseX - (Window::GetWidth() / 2)) / Window::GetWidth();

			auto newOrientation = glm::rotate(m_Orientation, glm::radians(-rotationX), glm::normalize(glm::cross(m_Orientation, m_UpDir)));

			if (!(glm::angle(newOrientation, m_UpDir) <= glm::radians(5.0f)) || glm::angle(newOrientation, -m_UpDir) <= glm::radians(5.0f))
			{
				m_Orientation = newOrientation;
			}

			m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotationY), m_UpDir);
			glfwSetCursorPos(Window::GetWindowPtr(), Window::GetWidth() / 2, Window::GetHeight() / 2);
		}
		if (Window::IsMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT))
		{
			// Simply shows the cursor again when the button for rotation has been released
			glfwSetInputMode(Window::GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_FirstClick = true;
		}
	}
}
