#include "Camera.h"
#include "../utils/Logger.h"

using namespace CoreWindow;

namespace CoreGraphics
{
	Camera::Camera(glm::vec3 position)
		: m_Position(position), m_Orientation(0.0f, 0.0f, -1.0f), m_UpDir(0.0f, 1.0f, 0.0f)
	{

	}

	void Camera::SetMatrix(float fov, float nearPlane, float farPlane, const Shader& shader, const char* uniform)
	{
		auto view = glm::identity<glm::mat4>();
		auto projection = glm::identity<glm::mat4>();

		view = glm::lookAt(m_Position, m_Position + m_Orientation, m_UpDir);
		projection = glm::perspective(glm::radians(fov), (float)(Window::GetWidth() / Window::GetHeight()), nearPlane, farPlane);

		glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
	}

	void Camera::CheckInput()
	{
		// To do: Implemented delta time between frames in order to slow down the movements

		if (Window::IsKeyPressed(GLFW_KEY_W))
		{
			m_Position += m_Speed * m_Orientation;
		}
		if (Window::IsKeyPressed(GLFW_KEY_A))
		{
			m_Position += m_Speed * -glm::normalize(glm::cross(m_Orientation, m_UpDir));
		}
		if (Window::IsKeyPressed(GLFW_KEY_S))
		{
			m_Position += m_Speed * -m_Orientation;
		}
		if (Window::IsKeyPressed(GLFW_KEY_D))
		{
			m_Position += m_Speed * glm::normalize(glm::cross(m_Orientation, m_UpDir));
		}

		if (Window::IsKeyPressed(GLFW_KEY_SPACE))
		{
			m_Position += m_Speed * m_UpDir;
		}
		if (Window::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			m_Position += m_Speed * -m_UpDir;
		}

		if (Window::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			m_Speed = 0.4f;
		}
		if (Window::IsKeyReleased(GLFW_KEY_LEFT_SHIFT))
		{
			m_Speed = 0.1f;
		}
	}
}
