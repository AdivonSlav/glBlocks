#pragma once

#include <chrono>

#include "Core.h"

using namespace CoreGraphics;
using namespace CoreWindow;
using namespace CoreGameObjects;
using namespace CoreUtils;

using namespace std::chrono;

#define NANO_TO_SECOND 1000000000.0

class Application
{
private:
	Window& m_Window;
	Renderer* m_Renderer;
	Camera m_Camera;
	ChunkManager* m_ChunkManager;
	Dashboard* m_Dashboard;
	TerrainGenerator* m_Generator;

	time_point<steady_clock> m_LastTime;
	double m_DeltaTime;
	double m_ElapsedTime;
	double m_FrameRate;
	double m_AverageFrameTime; // in milliseconds
	unsigned int m_FrameCount;
public:
	Application();
	~Application();

	/**
	 * \brief Starts the game loop
	 */
	void Run();

	/**
	 * \brief Calculates the completion time since the last frame
	 */
	void CalcTime();
};
