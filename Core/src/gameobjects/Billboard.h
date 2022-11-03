#pragma once

#include "../CoreAPI.h"

namespace CoreGameObjects
{
	class CORE_API Billboard
	{
	protected:
		glm::vec3 m_Position;
	public:
		Billboard();
		Billboard(glm::vec3 position);
		~Billboard() = default;

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }
	};
}
