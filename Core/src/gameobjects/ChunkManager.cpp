#include "ChunkManager.h"

namespace CoreGameObjects
{
	ChunkManager::ChunkManager()
	{
		m_LoadedChunks = new std::unordered_map<glm::vec3, Chunk*>();
		m_UnloadedChunks = new std::unordered_map<glm::vec3, const char*>();
	}

	ChunkManager::~ChunkManager()
	{
		for (auto& chunk : *m_LoadedChunks)
		{
			delete chunk.second;
		}

		delete m_LoadedChunks;
	}
}
