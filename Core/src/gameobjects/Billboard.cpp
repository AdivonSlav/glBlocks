#include "Billboard.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

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
