#pragma once

#include "Core.h"

using namespace CoreGraphics;
using namespace CoreWindow;

class Game
{
private:
	Window& m_Window;
	Renderer* m_Renderer;
	Camera m_Camera;

	double m_DeltaTime;
public:
	Game();
	~Game();

	/**
	 * \brief Starts the game loop
	 */
	void Run();
};
