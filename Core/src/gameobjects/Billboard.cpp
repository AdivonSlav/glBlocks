#include "Billboard.h"

namespace CoreGameObjects
{
	Billboard::Billboard()
		: m_Position(0.0f, 0.0f, 0.0f)
	{

	}

	Billboard::Billboard(glm::vec3 position)
		: m_Position(position)
	{

	}
}
