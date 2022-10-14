#include "ChunkManager.h"

namespace CoreGameObjects
{
	ChunkManager::ChunkManager()
	{
		// For testing purposes

		m_Chunks = new std::unordered_map<glm::vec3, Chunk*>();
	}

	ChunkManager::~ChunkManager()
	{
		for (auto& chunk : *m_Chunks)
		{
			delete chunk.second;
		}

		delete m_Chunks;
	}
}
