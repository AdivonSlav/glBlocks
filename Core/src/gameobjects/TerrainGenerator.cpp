#include "TerrainGenerator.h"

#include <glm/gtc/noise.hpp>

namespace CoreGameObjects
{
	TerrainGenerator::TerrainGenerator(ChunkManager& chunkManager)
		: m_ChunkManager(chunkManager)
	{

	}

	void TerrainGenerator::Generate()
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				auto chunk = new Chunk();
				glm::vec3 chunkPos(i * CHUNK_X, -CHUNK_Y, j * CHUNK_Z);

				m_ChunkManager.GetLoadedChunks().insert(std::pair(chunkPos, chunk));
			}
		}
	}
}
