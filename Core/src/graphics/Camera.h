#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "../window/Window.h"

namespace CoreGraphics
{
	class CORE_API Camera
	{
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Orientation;
		glm::vec3 m_UpDir;

		float m_Speed;
		float m_Sensitivity;

		bool m_FirstClick;
	public:
		Camera(glm::vec3 position);
		~Camera() = default;

		/**
		 * \brief Sets the relevant parameters of the perspective matrix of the camera
		 * \param fov Field of view of the camera
		 * \param nearPlane Objects closer than this will be clipped from view
		 * \param farPlane Objects further away than this will be clipped from view
		 * \param shader The shader to be used
		 * \param uniform The uniform to which the matrix will be sent in the shader
		 */
		void SetMatrix(float fov, float nearPlane, float farPlane, const Shader& shader, const char* uniform);

		/**
		 * \brief Checks to see if any input toward the camera has happened and processes it
		 */
		void CheckInput(double deltaTime);

		const glm::vec3& GetPosition() const { return m_Position; }
	};
}