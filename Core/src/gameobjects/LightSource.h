#pragma once

#include "Billboard.h"
#include "../graphics/VertexArray.h"
#include "../graphics/buffers/IndexBuffer.h"

namespace CoreGameObjects
{
	enum class LightSourceType { SUN, MOON };

	class CORE_API LightSource : public Billboard
	{
	private:
		CoreGraphics::VertexArray m_VAO;
		CoreGraphics::Buffers::IndexBuffer* m_IBO;

		float m_Size;

		uint8_t m_IndexCount;
	public:
		LightSource();
		LightSource(glm::vec3 position, float scale, LightSourceType type);
		~LightSource();

		const CoreGraphics::VertexArray& GetVAO() const { return m_VAO; }
		const CoreGraphics::Buffers::IndexBuffer* GetIBO() const { return m_IBO; }
		float GetSize() { return m_Size; }
		const uint8_t GetIndexCount() const { return m_IndexCount; }
	};
}