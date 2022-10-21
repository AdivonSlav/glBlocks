#pragma once

#include <glm/vec3.hpp>

#include "../CoreAPI.h"
#include "../graphics/Camera.h"

namespace CoreUtils
{
	class CORE_API Dashboard
	{
	private:
		bool m_Show;

		double m_FPS;
		double m_AverageFrameTime;

		CoreGraphics::Camera* m_Camera;
	public:
		Dashboard(bool show, CoreGraphics::Camera* camera);
		~Dashboard();

		/**
		 * \brief Prepares a new frame and renders the prepared ImGui window
		 */
		void Render();

		/**
		 * \brief Constructs the needed windows before calling Render()
		 */
		void PrepareWindows();

		/**
		 * \brief Sends the data needed for the ImGui windows to display
		 * \param fps FPS to display
		 * \param avgFrameTime Average frametime to display
		 */
		void SendData(double fps, double avgFrameTime);


	};


}
