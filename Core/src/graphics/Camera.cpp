
#include "Camera.h"
#include "../vendor/imgui/imgui.h"
#include "../utils/Logger.h"
#include "../utils/Dashboard.h"

using namespace CoreWindow;

namespace CoreGraphics
{
	Camera::Camera(glm::vec3 position)
		: m_Position(position), m_Sensitivity(20.0f), m_FirstClick(false)
	{
		m_Shaders.reserve(2);
		m_Orientation = glm::normalize(m_Position - glm::vec3(0.0f, 0.0f, 0.0f));

		// We can get the right vector of the camera by just getting the cross product of the up direction (in world space) and the direction where the camera is facing
		// The perpendicular vector to these two is just the right vector we need
		m_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_Orientation));

		// The up vector in camera space is again just the vector perpendicular to the direction and the right vector so we perform a cross product
		m_UpDir = glm::cross(m_Orientation, m_Right);
	}

	void Camera::SendShader(Shader& shader)
	{
		m_Shaders.push_back(&shader);
	}

	void Camera::SetMatrix(float fov, float nearPlane, float farPlane)
	{
		// Initializing view and projection matrices as identity matrices
		auto view = glm::identity<glm::mat4>();
		auto projection = glm::identity<glm::mat4>();

		// Calculates the view and projection matrices
		view = glm::lookAt(m_Position, m_Position + m_Orientation, m_UpDir);
		projection = glm::perspective(glm::radians(fov), (float)(Window::GetWidth() / (float)Window::GetHeight()), nearPlane, farPlane);

		m_RightWorld = { view[0][0], view[1][0], view[2][0] };
		m_UpDirWorld = { view[0][1], view[1][1], view[2][1] };

		for (auto& shader : m_Shaders)
		{
			shader->Bind();
			// Sends the matrix to the uniform variable located in the vertex shader
			shader->SetMat4<float>("uProjection", projection);
			shader->SetMat4<float>("uView", view);
			shader->Unbind();
		}
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
			m_Speed = 25.0f;
		}
		if (Window::IsKeyReleased(GLFW_KEY_LEFT_SHIFT))
		{
			m_Speed = 5.0f;
		}


		if (CoreUtils::Dashboard::IsShown())
		{
			glfwSetInputMode(Window::GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
			return;
		}

		glfwSetInputMode(Window::GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);

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
}
