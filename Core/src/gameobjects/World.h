#pragma once

#include "../CoreAPI.h"
#include "TerrainGenerator.h"
#include "LightSource.h"
#include "../utils/Timer.h"

namespace CoreGameObjects
{
	// A container object for the terrain generator and the light sources
	class CORE_API World
	{
	private:
		float m_SunMoonHeight;
		double m_CurrentDeclination;

		TerrainGenerator m_Generator;
		LightSource m_Sources[2];
	public:
		World();
		~World() = default;

		/**
		 * \brief Advances the day/night cycle and sets the correct sun/moon position
		 */
		void StepTime(const CoreUtils::Timer& timer);

		TerrainGenerator& GetGenerator() { return m_Generator; }
		LightSource& GetLightSource(LightSourceType type) { return m_Sources[(int)type]; }
	};
}