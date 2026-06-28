#include "PCH.h"

#include "Camera.h"
#include "../vendor/imgui/imgui.h"
#include "../utils/Dashboard.h"
#include "../window/Window.h"

using namespace CoreWindow;

namespace CoreGraphics
{
	static FrustumPlane NormalizePlane(const glm::vec4& plane)
	{
		float length = glm::length(glm::vec3(plane));
		return { glm::vec3(plane) / length, plane.w / length };
	}

	Camera::Camera() {}

	Camera::Camera(glm::vec3 position)
			: m_Position(position), m_Yaw(-90.0f), m_Pitch(0.0f), m_RotationSensitivity(0.06f), m_FirstInput(true)
	{
		m_Shaders.reserve(2);

		m_View = glm::identity<glm::mat4>();
		m_Orientation = glm::normalize(m_Position - glm::vec3(0.0f, 0.0f, 0.0f));

		// We can get the right vector of the camera by just getting the cross product of the up direction (in world space) and the direction where the camera is facing
		// The perpendicular vector to these two is just the right vector we need
		m_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_Orientation));

		// The up vector in camera space is again just the vector perpendicular to the direction and the right vector so we perform a cross product
		m_UpDir = glm::cross(m_Orientation, m_Right);

		m_LastMousePos = Window::GetCursorPos();
	}

	void Camera::SendShader(Shader &shader)
	{
		m_Shaders.push_back(&shader);
	}

	void Camera::OnUpdate(double deltaTime)
	{
		CheckInput(deltaTime);

		m_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_Orientation));
		m_UpDir = glm::normalize(glm::cross(m_Orientation, m_Right));

		m_View = glm::lookAt(m_Position, m_Position + m_Orientation, m_UpDir);
		m_ViewProjection = m_Perspective * m_View;
		UpdateFrustumPlanes();

		m_RightWorld = {m_View[0][0], m_View[1][0], m_View[2][0]};
		m_UpDirWorld = {m_View[0][1], m_View[1][1], m_View[2][1]};

		for (auto &shader : m_Shaders)
		{
			// Sends the matrices to the uniform variables located in the vertex shader
			shader->Bind();
			shader->SetMat4<float>("uProjection", m_Perspective);
			shader->SetMat4<float>("uView", m_View);
			shader->Unbind();
		}
	}

	void Camera::OnResize(int width, int height)
	{
		m_Perspective = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
		m_ViewProjection = m_Perspective * m_View;
		UpdateFrustumPlanes();
	}

	void Camera::UpdateFrustumPlanes()
	{
		const glm::mat4& m = m_ViewProjection;

		m_FrustumPlanes[0] = NormalizePlane(glm::vec4(m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0], m[3][3] + m[3][0]));
		m_FrustumPlanes[1] = NormalizePlane(glm::vec4(m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0], m[3][3] - m[3][0]));
		m_FrustumPlanes[2] = NormalizePlane(glm::vec4(m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1], m[3][3] + m[3][1]));
		m_FrustumPlanes[3] = NormalizePlane(glm::vec4(m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1], m[3][3] - m[3][1]));
		m_FrustumPlanes[4] = NormalizePlane(glm::vec4(m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2], m[3][3] + m[3][2]));
		m_FrustumPlanes[5] = NormalizePlane(glm::vec4(m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2], m[3][3] - m[3][2]));
	}

	void Camera::CheckInput(double deltaTime)
	{
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
			m_Speed = 15.0f;
		}
		if (Window::IsKeyReleased(GLFW_KEY_LEFT_SHIFT))
		{
			m_Speed = 5.0f;
		}

		if (CoreUtils::Dashboard::IsShown())
		{
			Window::SetCursorMode(CursorMode::NORMAL);
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
			return;
		}

		Window::SetCursorMode(CursorMode::DISABLED);
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);

		auto mousePos = Window::GetCursorPos();
		glm::vec2 delta;

		if (m_FirstInput)
		{
			m_LastMousePos = mousePos;
			m_FirstInput = false;
		}

		delta.x = mousePos.x - m_LastMousePos.x;
		delta.y = m_LastMousePos.y - mousePos.y;
		delta *= m_RotationSensitivity;
		m_LastMousePos = mousePos;

		if (delta.x != 0.0f || delta.y != 0.0f)
		{
			m_Yaw += delta.x;
			m_Pitch += delta.y;

			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;

			glm::vec3 newOrientation;
			newOrientation.x = std::cos(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch));
			newOrientation.y = std::sin(glm::radians(m_Pitch));
			newOrientation.z = std::sin(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch));

			m_Orientation = glm::normalize(newOrientation);
		}
	}
}
