#pragma once

#include "../CoreAPI.h"

#include "../graphics/Camera.h"

namespace CoreUtils
{
	struct CORE_API Payload
	{
		float timeStepMultiplier = 1.0f;
	};

	class CORE_API Dashboard
	{
	private:
		static bool m_Show;

		double m_FPS;
		double m_AverageFrameTime;
		static bool m_GenNewWorld;

		static Payload m_Payload;

		CoreGraphics::Camera* m_Camera;
	public:
		Dashboard(CoreGraphics::Camera* camera);
		~Dashboard() = default;

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
		void GetData(double fps, double avgFrameTime);

		/**
		 * \brief Sends data from the dashboard
		 * \return Data from the Dashboard
		 */
		static Payload& GetPayload() { return m_Payload; }

		/**
		 * \brief Destroys the ImGui context
		 */
		static void Cleanup();

		static void SetShown(bool shown) { m_Show = shown; }
		static bool IsShown() { return m_Show;}
		static bool ShouldGenerateWorld() { return m_GenNewWorld; }
	};


}
