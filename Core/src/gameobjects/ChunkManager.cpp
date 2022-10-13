#include "ChunkManager.h"

namespace CoreGameObjects
{
	ChunkManager::ChunkManager()
	{
		// For testing purposes

		m_Chunks = new std::unordered_map<glm::vec3, Chunk*>();

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				auto chunk = new Chunk();
				glm::vec3 chunkPos(i * CHUNK_X,-CHUNK_Y, j * CHUNK_Z);

				m_Chunks->insert(std::pair(chunkPos, chunk));
			}
		}
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
