#include <cmath>

#include "World.h"
#include "../utils/Dashboard.h"

namespace CoreGameObjects
{
	World::World()
		: m_SunMoonHeight(500.0f), m_CurrentDeclination(0.0),
			m_Sources{{glm::vec3(0.0f, m_SunMoonHeight, 0.0f), 4.0f, LightSourceType::SUN}, {glm::vec3(0.0f, m_SunMoonHeight, 0.0f), 4.0f, LightSourceType::MOON}}
	{
		
	}

	void World::StepTime(const CoreUtils::Timer& timer)
	{
		glm::vec3 newPos;

		m_CurrentDeclination += timer.GetDelta() * (double)CoreUtils::Dashboard::GetPayload().timeStepMultiplier;
		double newDecl = glm::radians(m_CurrentDeclination);

		newPos.x = m_SunMoonHeight * std::cos(newDecl);
		newPos.y = m_SunMoonHeight * std::sin(newDecl);
		newPos.z = 0.0f;

		m_Sources[(int)LightSourceType::SUN].SetPosition(newPos);
		m_Sources[(int)LightSourceType::MOON].SetPosition(-newPos);

		if (m_CurrentDeclination >= 359.0)
			m_CurrentDeclination = 0.0;
	}
}
