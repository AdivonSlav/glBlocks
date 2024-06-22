#pragma once

#include "../CoreAPI.h"

#include "Shader.h"

namespace CoreGraphics
{
	class CORE_API Camera
	{
	private:
		std::vector<Shader *> m_Shaders;

		glm::mat4 m_Perspective;
		glm::mat4 m_View;
		glm::mat4 m_ViewProjection;

		glm::vec3 m_Position;
		glm::vec3 m_Orientation;
		glm::vec3 m_Right;
		glm::vec3 m_UpDir;

		glm::vec3 m_RightWorld;
		glm::vec3 m_UpDirWorld;

		float m_Yaw;
		float m_Pitch;

		float m_Speed;
		float m_RotationSensitivity;
		bool m_FirstInput;

		glm::vec2 m_LastMousePos;

	public:
		Camera();
		Camera(glm::vec3 position);
		~Camera() = default;

		/**
		 * \brief Sends a shader that is stored as a pointer in the camera so the proper matrices can be applied
		 * \param shader The shader that is to be sent
		 */
		void SendShader(Shader &shader);

		/**
		 * \brief Called every frame to recalculate the view matrices and send them to shader uniforms
		 */
		void OnUpdate(double deltaTime);

		/**
		 * \brief Called on window resize to recalculate the perspective matrix
		 * \param width New width of the window
		 * \param height New height of the window
		 */
		void OnResize(int width, int height);

		/**
		 * \brief Checks to see if any input toward the camera has happened and processes it
		 */
		void CheckInput(double deltaTime);

		const glm::mat4 &GetPerspective() const { return m_Perspective; }
		const glm::mat4 &GetView() const { return m_View; }
		const glm::mat4 &GetViewProjection() const { return m_ViewProjection; }
		const glm::vec3 &GetPosition() const { return m_Position; }
		const glm::vec3 &GetWorldRight() const { return m_RightWorld; }
		const glm::vec3 &GetWorldUp() const { return m_UpDirWorld; }

		void SetPosition(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
	};
}