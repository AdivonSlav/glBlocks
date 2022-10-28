#include <cmath>

#include "World.h"
#include "../utils/Dashboard.h"
#include "../utils/Maths.h"

namespace CoreGameObjects
{
	World::World()
		: m_SunMoonHeight(500.0f), m_CurrentDeclination(0.0), m_SunHasSet(false),
			m_Sources{{glm::vec3(0.0f, m_SunMoonHeight, 0.0f), 4.0f, LightSourceType::SUN}, {glm::vec3(0.0f, m_SunMoonHeight, 0.0f), 4.0f, LightSourceType::MOON}}
	{
		m_ClearColor = glm::vec3(CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B);
	}

	void World::StepTime(const CoreUtils::Timer& timer)
	{
		glm::vec3 newPos;

		m_CurrentDeclination += timer.GetDelta() * (double)CoreUtils::Dashboard::GetPayload().timeStepMultiplier;
		double newDecl = glm::radians(m_CurrentDeclination);

		newPos.x = m_SunMoonHeight * std::cos(newDecl);
		newPos.y = m_SunMoonHeight * std::sin(newDecl) + 100.0f;
		newPos.z = 0.0f;

		m_Sources[(int)LightSourceType::SUN].SetPosition(newPos);
		m_Sources[(int)LightSourceType::MOON].SetPosition(-newPos);

		if (m_CurrentDeclination >= 359.0)
			m_CurrentDeclination = 0.0;
	}

	float World::GetLightLevel()
	{
		if (m_CurrentDeclination >= 0.0 && m_CurrentDeclination <= 90.0)
		{
			m_SunHasSet = false;
			float interpolatedValue = Interpolate<float>(m_CurrentDeclination, 0.0f, 90.0f, 0.0f, 1.0f);

			CalculateClearColor(interpolatedValue);

			return interpolatedValue;
		}

		if (m_CurrentDeclination > 90.0 && m_CurrentDeclination <= 180.0)
		{
			m_SunHasSet = false;
			float interpolatedValue = Interpolate<float>(m_CurrentDeclination, 180.0f, 90.0f, 0.0f, 1.0f);

			CalculateClearColor(interpolatedValue);

			return interpolatedValue;
		}

		if (m_CurrentDeclination > 180.0 && m_CurrentDeclination <= 270.0)
		{
			m_SunHasSet = true;
			float interpolatedValue = Interpolate<float>(m_CurrentDeclination, 180.0, 270.0, 0.0f, 1.0f);

			CalculateClearColor(interpolatedValue);

			return interpolatedValue;
		}

		if (m_CurrentDeclination > 270.0 && m_CurrentDeclination <= 359.0)
		{
			m_SunHasSet = true;
			float interpolatedValue = Interpolate<float>(m_CurrentDeclination, 359.0, 270.0, 0.0f, 1.0f);

			CalculateClearColor(interpolatedValue);

			return interpolatedValue;
		}
	}

	const glm::vec3& World::GetLightPosition() const
	{
		return m_SunHasSet? m_Sources[(int)LightSourceType::MOON].GetPosition(): m_Sources[(int)LightSourceType::SUN].GetPosition();
	}

	void World::CalculateClearColor(float interpolated)
	{
		float red = Interpolate<float>(interpolated, 0.0f, 1.0f, 16.0f, CLEAR_COLOR_R);
		float green = Interpolate<float>(interpolated, 0.0f, 1.0f, 24.0f, CLEAR_COLOR_G);
		float blue = Interpolate<float>(interpolated, 0.0f, 1.0f, 31.0f, CLEAR_COLOR_B);

		m_ClearColor = glm::vec3(red, green, blue);
	}

	glm::vec3 World::GetClearColor() const
	{
		return {m_ClearColor.r / 255.0f, m_ClearColor.g / 255.0f, m_ClearColor.b / 255.0f};
	}
}
