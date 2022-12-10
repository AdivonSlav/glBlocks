#include "VertexArrayManager.h"
#include "../utils/Logger.h"

namespace CoreGraphics
{
	std::queue<VertexArray*> VertexArrayManager::m_Pool;

	void VertexArrayManager::CacheVAO(VertexArray* vao)
	{
		if (vao == nullptr)
		{
			LOG_ERROR("Tried to cache nullpointed VAO!");
			return;
		}

		m_Pool.push(vao);
		LOG_INFO("Cached VAO -> " << m_Pool.size());
	}

	VertexArray* VertexArrayManager::GetVAO()
	{
		auto vao = m_Pool.front();
		m_Pool.pop();
		LOG_INFO("Popped VAO -> " << m_Pool.size());

		if (vao == nullptr)
		{
			LOG_ERROR("Tried to return nullpointed VAO for usage!");
		}

		return vao;
	}

	bool VertexArrayManager::IsEmpty()
	{
		return m_Pool.empty();
	}

	void VertexArrayManager::Cleanup()
	{
		for (int i = 0; i < m_Pool.size(); i++)
		{
			auto vao = m_Pool.front();
			delete vao;
			m_Pool.pop();
		}
	}
}
