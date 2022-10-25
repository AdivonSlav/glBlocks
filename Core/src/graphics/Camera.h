#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "../window/Window.h"

namespace CoreGraphics
{
	class CORE_API Camera
	{
	private:
		std::vector<Shader*> m_Shaders;

		glm::vec3 m_Position;
		glm::vec3 m_Orientation;
		glm::vec3 m_Right;
		glm::vec3 m_UpDir;

		glm::vec3 m_RightWorld;
		glm::vec3 m_UpDirWorld;

		float m_Speed;
		float m_Sensitivity;

		bool m_FirstClick;
	public:
		Camera(glm::vec3 position);
		~Camera() = default;

		/**
		 * \brief Sends a shader that is stored as a pointer in the camera so the proper matrices can be applied
		 * \param shader The shader that is to be sent
		 */
		void SendShader(Shader& shader);

		/**
		 * \brief Sets the relevant parameters of the perspective matrix of the camera
		 * \param fov Field of view of the camera
		 * \param nearPlane Objects closer than this will be clipped from view
		 * \param farPlane Objects further away than this will be clipped from view
		 */
		void SetMatrix(float fov, float nearPlane, float farPlane);

		/**
		 * \brief Checks to see if any input toward the camera has happened and processes it
		 */
		void CheckInput(double deltaTime);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }

		const glm::vec3& GetWorldRight() const { return m_RightWorld; }
		const glm::vec3& GetWorldUp() const { return m_UpDirWorld; }
	};
}