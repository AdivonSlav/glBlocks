#pragma once

#include "../graphics/VertexArray.h"
#include "../graphics/buffers/IndexBuffer.h"

#include "glm/glm.hpp"

using namespace  CoreGraphics;
using namespace  CoreGraphics::Buffers;

namespace CoreGameObjects
{
	enum class BlockType { AIR, DIRT};

	class CORE_API Block
	{
	private:
		BlockType m_Type;

		VertexArray* m_VAO;
		IndexBuffer* m_IBO;

		glm::vec3 m_Position;
		float m_Scale;
	public:
		Block(BlockType type, const glm::vec3& position, float scale);
		~Block();

		VertexArray* GetVAO() { return m_VAO; }
		IndexBuffer* GetIBO() { return m_IBO; }
	};
}