#pragma once

#include "Core.h"

using namespace CoreGraphics;
using namespace CoreWindow;
using namespace CoreGameObjects;

class Game
{
private:
	Window& m_Window;
	Renderer* m_Renderer;
	Camera m_Camera;
	ChunkManager* m_ChunkManager;

	double m_DeltaTime;
public:
	Game();
	~Game();

	/**
	 * \brief Starts the game loop
	 */
	void Run();
};
