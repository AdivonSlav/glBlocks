#pragma once

#include "../CoreAPI.h"
#include "TerrainGenerator.h"
#include "LightSource.h"

#define CLEAR_COLOR_R 125.0f // Red channel max value of glClearColor
#define CLEAR_COLOR_G 206.0f // Green channel max value of glClearColor
#define CLEAR_COLOR_B 235.0f // Blue channel max value of glClearColor

namespace CoreGameObjects
{
	// A container object for the terrain generator and the light sources
	class CORE_API World
	{
	private:
		float m_SunMoonHeight;
		double m_CurrentDeclination;
		bool m_SunHasSet;

		glm::vec3 m_ClearColor;

		TerrainGenerator m_Generator;
		LightSource m_Sources[2];
	public:
		World();
		~World() = default;

		/**
		 * \brief Advances the day/night cycle and sets the correct sun/moon position
		 */
		void StepTime(const CoreUtils::Timer& timer);

		/**
		 * \brief Calculates the light level needed by the shader based on sun/moon declination
		 * \return The light level between 0.0f and 1.0f
		 */
		float GetLightLevel();

		/**
		 * \brief Returns the position of the light source currently above the horizon
		 * \return The position of the light source
		 */
		const glm::vec3& GetLightPosition() const;

		/**
		 * \brief Calculates the clear color value based on the light level
		 * \param interpolated The interpolated value of the light level
		 */
		void CalculateClearColor(float interpolated);

		TerrainGenerator& GetGenerator() { return m_Generator; }
		LightSource& GetLightSource(LightSourceType type) { return m_Sources[(int)type]; }
		bool GetHasSet() const { return m_SunHasSet;}
		glm::vec3 GetClearColor() const;
	};
}