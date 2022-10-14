#pragma once

#include "../CoreAPI.h"

namespace CoreUtils
{
	class CORE_API Dashboard
	{
	private:
		bool m_Show;

		double m_FPS;
		double m_AverageFrameTime;
	public:
		Dashboard(bool show);
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
