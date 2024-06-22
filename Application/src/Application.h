#pragma once

#include "Core.h"

using namespace CoreGraphics;
using namespace CoreWindow;
using namespace CoreGameObjects;
using namespace CoreUtils;

class Application
{
private:
	Window &m_Window;
	Renderer *m_Renderer;
	Camera m_Camera;
	Dashboard *m_Dashboard;
	World *m_World;
	Timer m_Timer;

	double m_FrameRate;
	double m_AverageFrameTime; // in milliseconds
	unsigned int m_FrameCount;
	double m_LastUpdate;

public:
	Application();
	~Application();

	/**
	 * \brief Starts the main window loop
	 */
	void Run();

	/**
	 * \brief Calculates the performance at the end of each frame
	 */
	void CalcPerf();
};
